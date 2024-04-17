#ifndef FILTER_H
#define FILTER_H

#include "lib/p_queue.h"
#include "tasks.h"

struct filter_member_vars {
    p_queue* free_queue;
    pthread_mutex_t* mutex;
    thread_params* tps;
    const size_t threadsCount; 
};
typedef struct filter_member_vars filter_member_vars;

void apply_filter(elementary_task etask, size_t number_of_threads);

filter_member_vars construct_member_vars(const size_t threadsCount);

void initialize_entities(filter_member_vars* member_vars);

void initialize_free_queue(filter_member_vars* member_vars);

void initialize_mutex(filter_member_vars* member_vars);

void initialize_tps(filter_member_vars* member_vars);

void delegate_tasks(filter_member_vars member_vars, elementary_task etask);

void prepare_entities_for_next_iteration(filter_member_vars member_vars, size_t* free_process, thread_params** tp, pthread_t threads[]);

void join_threads_and_free_vars(filter_member_vars member_vars, pthread_t threads[], const size_t tasks_completed);

void full_queue(p_queue* free_queue, const size_t threadsCount);

#define ALL_TASKS_COMPLETED 0
#define KEEP_WORKING 1
int8_t prepare_next_etask(elementary_task* etask);

#endif