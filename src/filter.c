#include <stdio.h>
#include "macroses.h"
#include "filter.h"

void apply_filter(elementary_task etask, const size_t threadsCount) {
    filter_member_vars member_vars = construct_member_vars(threadsCount);

    delegate_tasks(member_vars, etask);
}

filter_member_vars construct_member_vars(const size_t threadsCount) {
    filter_member_vars member_vars = { .threadsCount = threadsCount };
    initialize_entities(&member_vars);
    return member_vars;
}

void initialize_entities(filter_member_vars* member_vars) {
    initialize_free_queue(member_vars);

    initialize_mutex(member_vars);

    initialize_tps(member_vars);
}

void initialize_free_queue(filter_member_vars* member_vars) {
    member_vars->free_queue = (p_queue*)malloc(sizeof(p_queue));
    ASSERT_ALLOCATED(member_vars->free_queue, "Error: filter.c, initialize_free_queue, 26: bad alloc!");
    *member_vars->free_queue = create_queue(0, 0);
    full_queue(member_vars->free_queue, member_vars->threadsCount);
}

void initialize_mutex(filter_member_vars* member_vars) {    
    member_vars->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    ASSERT_ALLOCATED(member_vars->mutex, "Error: filter.c, initialize_mutex, 33: bad alloc!");
    pthread_mutex_init(member_vars->mutex, NULL);
}

void initialize_tps(filter_member_vars* member_vars) {
    member_vars->tps = (thread_params*)malloc(member_vars->threadsCount * sizeof(thread_params));
    ASSERT_ALLOCATED(member_vars->tps, "Error: filter.c, initialize_tps, 39: bad alloc!");
}

void delegate_tasks(filter_member_vars member_vars, elementary_task etask) {
    thread_task_args* args = (thread_task_args*)malloc(sizeof(thread_task_args));
    ASSERT_ALLOCATED(args, "Error, filter.c, delegate_tasks, 44: bad alloc!\n");
    pthread_t* threads = (pthread_t*)malloc(member_vars.threadsCount * sizeof(pthread_t));
    ASSERT_ALLOCATED(threads,  "Error, filter.c, delegate_tasks, 46: bad alloc!\n");
    thread_params* tp;
    size_t free_process, tasks_complited = 0;

    while(1) {
        prepare_entities_for_next_iteration(
            member_vars,
            &free_process,
            &tp,    
            threads
        );
        *args = (thread_task_args) { .task = etask, .tp = tp };
        execute_task(args);
        ++tasks_complited;
        if (prepare_next_etask(&etask) == ALL_TASKS_COMPLETED) {
            goto out_of_delegating_loop;
        }
        while (is_empty_queue(member_vars.free_queue)) {}
    }

    out_of_delegating_loop:
    join_threads_and_free_vars(member_vars, threads, tasks_complited);
}

void prepare_entities_for_next_iteration(filter_member_vars member_vars, size_t* free_process, thread_params** tp, pthread_t threads[]) {
    pthread_mutex_lock(member_vars.mutex);
    *free_process = q_pop(member_vars.free_queue);
    pthread_mutex_unlock(member_vars.mutex);
    *tp = &member_vars.tps[*free_process];
    **tp = (thread_params){ 
        .pid = &threads[*free_process], 
        .id = *free_process, 
        .free_queue = member_vars.free_queue,
        .mutex = member_vars.mutex
    };
}

void join_threads_and_free_vars(filter_member_vars member_vars, pthread_t* threads, const size_t tasks_completed) {
    size_t threadsUsed = (member_vars.threadsCount < tasks_completed ? 
        member_vars.threadsCount :
        tasks_completed
    );
    //printf("Treads count %ld, tasks complited %ld\n", member_vars.threadsCount, tasks_completed);
    //printf("Threads used %ld\n", threadsUsed);
    for (size_t i = 0; i < threadsUsed; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            ERROR(
                "Error: can not join process",
                RUNTIME_ERROR
            );
        }
        //printf("%ld process joined\n", i);
    }
    pthread_mutex_destroy(member_vars.mutex);
    q_clear(member_vars.free_queue);
    free(member_vars.free_queue);
    free(member_vars.mutex);
    free(member_vars.tps);
    free(threads);
}

void full_queue(p_queue* free_queue, const size_t threadsCount) {
    q_clear(free_queue);
    for (size_t i = 0; i < threadsCount; ++i) {
        q_push(free_queue, i);
    }
}

int8_t prepare_next_etask(elementary_task* etask) {
    if (etask->j < etask->origin->m - STRUCTURE_ELEMENT_SIZE) {
        etask->j++;
        return KEEP_WORKING;
    } else if (etask->i < etask->origin->n - STRUCTURE_ELEMENT_SIZE) {
        etask->i++;
        etask->j = 0;
        return KEEP_WORKING;
    } else {
        return ALL_TASKS_COMPLETED;
    }
}