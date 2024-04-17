#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include "planner.h"


void planner(tech_data* data, tech_params params) {
// Copy data
    tech_data* erosion = copy_tech_data(data);
    tech_data* extention = copy_tech_data(data);

    if (erosion == NULL || extention == NULL) {
        ERROR(
            "Error: planner.c, planner: can not copy original image!\n",
            RUNTIME_ERROR
        );
    }

    if (data->n < STRUCTURE_ELEMENT_SIZE || data->m < STRUCTURE_ELEMENT_SIZE) {
        free(erosion);
        free(extention);
        return;
    }

// Create and initialize task propreties
    elementary_task etask = { 
        .origin = data,
        .erosion = erosion, 
        .extention = extention,
        .i = 0, .j = 0 
    };

// Apply changes K times
    for (size_t i = 0; i < params.k; ++i) {
        apply_filter(etask, params.threadsCount);
    }

// Writing reslut
    write_result(erosion, "erosion");
    write_result(extention, "extention");

// Free data
    free_tech_data(erosion);
    free(erosion);
    free_tech_data(extention);
    free(extention);
}