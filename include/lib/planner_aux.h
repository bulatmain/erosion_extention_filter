#ifndef PLANNER_AUX_H

#define PLANNER_AUX_H

#include "lib/p_queue.h"
#include "macroses.h"
#include "tasks.h"

tech_data* copy_tech_data(tech_data* data);

tech_data read_tech_data(const char* filename, const char* n, const char* m);

tech_params read_tech_params(const char* k_str, const char* threadsCount_str);

void print_tech_data(tech_data* data);

void write_result(tech_data* data, const char* filename);

void write_another_number(FILE* fd, float d);

void write_last_number(FILE* fd, float d);

void free_tech_data(tech_data* data);

#endif