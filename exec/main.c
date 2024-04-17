#include <stdio.h>
#include <sys/time.h>
#include "planner.h"
#include "error_messages.h"

void write_results_to_file(const char* filename, size_t result) {
    FILE* fp = fopen(filename, "a");
    if (fp == NULL) {
        ERROR(
            "Error: main.c, write_result_to_file: unable to open file\n",
            RUNTIME_ERROR
        );
    }

    if (fprintf(fp, "%ld\n", result) < 0) {
        ERROR(
                "ERROR: main.c, write_result_to_file: can not write into file.\n",
                RUNTIME_ERROR
        );
    }

    if (fclose(fp) < 0) {
        ERROR(
            "ERROR: main.c, write_results_to_file: can not close file\n",
            RUNTIME_ERROR
        )
    }

}

int main(int argc, char** argv) {

    if (argc != 7) {
        ERROR(
            INVALID_ARGIMENTS_ERROR_MESSSAGE,
            INVALID_ARGUMENTS_ERROR
        );
    } 

    tech_data* data = (tech_data*)malloc(sizeof(tech_data));
    ASSERT_ALLOCATED(data, "Error: main.c, main, 15: bad alloc!\n");
    *data = read_tech_data(argv[1], argv[2], argv[3]);
    tech_params params = read_tech_params(argv[4], argv[5]);

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    planner(data, params);

    gettimeofday(&stop, NULL);
    size_t working_time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    //printf("took %lu us\n", working_time);

    write_results_to_file(argv[6], working_time);

    free_tech_data(data);
    free(data);

    return 0;
}