#include "planner_aux.h"

tech_data* copy_tech_data(tech_data* data) {
    if (data == NULL) {
        return NULL;
    }
    tech_data* copy = (tech_data*)malloc(sizeof(tech_data));
    if (copy == NULL) {
        ERROR(
            "Error: planner_aux.c, copy_tech_data: bad alloc\n",
            RUNTIME_ERROR
        );
    }
    if (data->image == NULL) {
        copy->image = NULL;
    } else {
        copy->image = (float**)malloc(data->n * sizeof(float*));    
        if (copy->image == NULL) {
            ERROR(
                "Error: planner_aux.c, copy_tech_data: bad alloc\n",
                RUNTIME_ERROR
            );
        }
        for (size_t i = 0; i < data->n; ++i) {
            copy->image[i] = (float*)malloc(data->m * sizeof(float));
            if (copy->image[i] == NULL) {
                ERROR(
                    "Error: planner_aux.c, copy_tech_data: bad alloc\n",
                    RUNTIME_ERROR
                );
            }
            for (size_t j = 0; j < data->m; ++j) {
                copy->image[i][j] = data->image[i][j];
            }
        }
    }
    copy->n = data->n;
    copy->m = data->m;
    return copy;
}

void read_float_from_file(FILE** fp, float* f) {
    if (fscanf(*fp, "%f", f) == 0) {
        ERROR(
            "Error: planner_aux.c, read_float_from_file: can not read float from file\n",
            RUNTIME_ERROR
        );
    }
}

tech_data read_tech_data(const char* filename, const char* n_str, const char* m_str) {
    size_t n, m;
    sscanf(n_str, "%zu", &n);
    sscanf(m_str, "%zu", &m);

    if (n < STRUCTURE_ELEMENT_SIZE || m < STRUCTURE_ELEMENT_SIZE) {
        ERROR(
            "Error: planner_aux.c, read_tech_data: passing image with size less than size of structure element!\n",
            INVALID_ARGUMENTS_ERROR
        );
    }
    
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        ERROR(
            "Error: planner_aux.c, read_tech_data: can not open file\n",
            RUNTIME_ERROR
        );
    }

    float** matrix = (float**)malloc(n * sizeof(float*));
    ASSERT_ALLOCATED(matrix, "Error: planner_aux.c, read_tech_data: bad alloc!\n");
    for (size_t i = 0; i < n; ++i) {
        matrix[i] = (float*)malloc(m * sizeof(float));
        ASSERT_ALLOCATED(matrix[i], "Error: planner_aux.c, read_tech_data: bad alloc!\n");
        for (size_t j = 0; j < m; ++j) {
            read_float_from_file(&fp, &matrix[i][j]);
        }
    }

    if (fclose(fp) < 0) {
        ERROR(
            "ERROR: planner_aux.c, read_tech_data: can not close file\n",
            RUNTIME_ERROR
        )
    }

    return (tech_data){ .image = matrix, .n = n, .m = m };
}

tech_params read_tech_params(const char* k_str, const char* threadsCount_str) {
    size_t k, threadsCount;
    sscanf(k_str, "%zu", &k);
    sscanf(threadsCount_str, "%zu", &threadsCount);

    if (threadsCount == 0) {
        ERROR(
            "Error: planner_aux.c, read_tech_params: invalid argument! threadsCount cat not be 0!\n",
            INVALID_ARGUMENTS_ERROR
        );
    }

    return (tech_params){ .k = k, .threadsCount = threadsCount };
}

void print_tech_data(tech_data* data) {
    for (size_t i = 0; i < data->n; ++i) {
        for (size_t j = 0; j < data->m; ++j) {
            printf("%f ", data->image[i][j]);
        }
        printf("\n");
    }
}

void write_result(tech_data* data, const char* filename) {
    FILE* fd = fopen(filename, "w");
    if (fd == NULL) {
        ERROR(
            "ERROR: planner_aux.c, write_result: can not create file.\n",
            RUNTIME_ERROR
        )
    }

    for (size_t i = 0; i < data->n; ++i) {
        for (size_t j = 0; j < data->m - 1; ++j) {
            write_another_number(fd, data->image[i][j]);
        }   
        write_last_number(fd, data->image[i][data->m - 1]);
    }

    if (fclose(fd) < 0) {
        ERROR(
            "ERROR: planner_aux.c, write_result: can not close file\n",
            RUNTIME_ERROR
        )
    }
}

void write_another_number(FILE* fd, float d) {
    if (fprintf(fd, "%f ", d) < 0) {
        ERROR(
            "ERROR: planner_aux.c, write_another_number: can not write into file.\n",
            RUNTIME_ERROR
        );
    }
}

void write_last_number(FILE* fd, float d) {
    if (fprintf(fd, "%f\n", d) < 0) {
        ERROR(
            "ERROR: planner_aux.c, write_last_number: can not write into file.\n",
            RUNTIME_ERROR
        );
    }
}

void free_tech_data(tech_data* data) {
    for (size_t i = 0; i < data->n; ++i) {
        free(data->image[i]);
    }
    free(data->image);
}
