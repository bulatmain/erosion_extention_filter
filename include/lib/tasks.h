#ifndef TASKS_H

#define TASKS_H

#include <pthread.h>
#include "lib/p_queue.h"
#include "macroses.h"

#ifndef STRUCTURE_ELEMENT_SIZE
#define STRUCTURE_ELEMENT_SIZE 3
#endif

struct tech_data
{
    float** image;
    size_t n;
    size_t m;
};
typedef struct tech_data tech_data;

struct tech_params
{
    const size_t k;
    const size_t threadsCount;
};
typedef struct tech_params tech_params;

struct elementary_task {
    tech_data* origin;
    tech_data* erosion;
    tech_data* extention;
    size_t i;
    size_t j;
};
typedef struct elementary_task elementary_task;

struct thread_params {
    pthread_t* pid;
    size_t id;
    p_queue* free_queue;
    pthread_mutex_t* mutex;
};
typedef struct thread_params thread_params;

struct thread_task_args {
    elementary_task task;
    thread_params* tp;
};
typedef struct thread_task_args thread_task_args;

void execute_task(thread_task_args* args);

void* calculate_elementary_task(void* thread_task_args);

void calculate_min_and_max(elementary_task task, float* min, float* max);

void write_pixel(elementary_task task, float min, float max);

#endif