#include <stdio.h>
#include "tasks.h"

void execute_task(thread_task_args* args) {
    if (pthread_create((args->tp->pid), NULL, calculate_elementary_task, (void *)args) != 0) {
        ERROR(
            "Error: can not create thread!\n",
            RUNTIME_ERROR
        );
    }
}

void* calculate_elementary_task(void* _args) {
    thread_task_args* args = (thread_task_args*)_args;
    thread_params* tp = (thread_params*)args->tp;

    float min, max;
    calculate_min_and_max(args->task, &min, &max);

    pthread_mutex_lock(tp->mutex);
        write_pixel(args->task, min, max);
        q_push(tp->free_queue, tp->id);
    pthread_mutex_unlock(tp->mutex);

    //printf("Thread runned with id %ld and pid %ld\n", tp->id, *tp->pid);
    pthread_exit(0);
}

void calculate_min_and_max(elementary_task task, float* min, float* max) {
    float minimal = 1, maximal = 0;
    float** origin_image = task.origin->image;
    for (size_t i = 0; i < STRUCTURE_ELEMENT_SIZE; ++i) {
        for (size_t j = 0; j < STRUCTURE_ELEMENT_SIZE; ++j) {
            if (origin_image[i + task.i][j + task.j] < minimal) {
                minimal = origin_image[i + task.i][j + task.j];
            }
            if (maximal < origin_image[i + task.i][j + task.j]) {
                maximal = origin_image[i + task.i][j + task.j];
            }
        }
    }
}

void write_pixel(elementary_task task, float min, float max) {    
    size_t central_pixel_y = task.i + STRUCTURE_ELEMENT_SIZE / 2;
    size_t central_pixel_x = task.j + STRUCTURE_ELEMENT_SIZE / 2;

    task.erosion->image[central_pixel_y][central_pixel_x] = min;
    task.extention->image[central_pixel_y][central_pixel_x] = max;
}