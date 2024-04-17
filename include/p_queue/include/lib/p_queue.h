#ifndef P_QUEUE

#define P_QUEUE

#include <inttypes.h>

typedef struct item
{
    int64_t data;
    struct item* next;
} item;

typedef struct p_queue
{
    uint64_t size;
    item* first;
    item* last;
} p_queue;

p_queue create_queue(const uint64_t size, const int64_t data);

int8_t is_empty_queue(const p_queue* q);

void q_push(p_queue* q, int64_t data);

int64_t q_pop(p_queue* q);

int64_t* q_at(const p_queue* q, const uint64_t index);

uint64_t q_size(const p_queue* q);

uint64_t q_count(const p_queue* q);

int8_t q_continuity(const p_queue* q);

int8_t q_valid(const p_queue* q);

void q_clear(p_queue* q);

item* q_create_item(const int64_t data);

#endif