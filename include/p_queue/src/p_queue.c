#include <stdlib.h>
#include <stdio.h>

#include "lib/p_queue.h"
#include "lib/macroses.h"
#include "lib/error_codes.h"


// Creates only one int64_t item allocq_ated in heap and return it's poiter.
item* q_create_item(const int64_t data) {
    item* p = (item*)malloc(sizeof(item));
    if (p == NULL) {
        ERROR(
            "Fatal in p_queue.c, q_create_item, 6: failed to allocate memory.\n",
            BAD_ALLOCATION_ERROR
        )
    }
    p->data = data;
    p->next = NULL;
    return p;
}

// Creates a queue with given size. Elements are initialized with zeros. 
p_queue create_queue(const uint64_t size, const int64_t data) {
    p_queue q;
    if (size > 0) {
        q.first = q_create_item(data);
    } else {
        q.first = NULL;
    }
    q.last = q.first;
    for (uint64_t i = 1; i < size; ++i) {
        q.last->next = q_create_item(data);
        q.last = q.last->next;
    }
    q.size = size;
    return q;
}

// Checks if queue has elements in it.
int8_t is_empty_queue(const p_queue* q) {
    return (q->first == q->last) && (q->first == NULL);
}

// Pushes a new element into the end of queue
void q_push(p_queue* q, int64_t data) {
    if (is_empty_queue(q)) {
        q->last = q_create_item(data);
        q->first = q->last;
    }
    else {
        q->last->next = q_create_item(data);
        q->last = q->last->next;
    }
    ++q->size;
}

// Pops an element from the beginning of queue and returns it's data. 
int64_t q_pop(p_queue* q) {
    if (is_empty_queue(q)) {
        ERROR(
            "Fatal in p_queue.c, q_pop, 61: pop from empty queue error.\n",
            POP_FROM_EMPTY_QUEUE_ERROR
        )
    }
    int64_t data = q->first->data;
    item* p = q->first;
    q->first = q->first->next;
    free(p);
    --q->size;
    if (q->size == 0) {
        q->first = q->last = NULL;
    }
    return data;
}

// Returns a pointer to value of element q_at given index.
int64_t* q_at(const p_queue* q, const uint64_t index) {
    if (index >= q->size) {
        return NULL;
    }
    item* iterator = q->first;
    uint64_t count = 0;
    while (count < index && iterator != NULL) {
        iterator = iterator->next;
        ++count;
    }
    if (count < index) {
        return NULL;
    }
    return &iterator->data;
}

uint64_t q_size(const p_queue* q) {
    return q->size;
}

uint64_t q_count(const p_queue* q) {
    uint64_t count = 0;
    item* iterator = q->first;
    while (iterator != NULL) {
        iterator = iterator->next;
        ++count;
    }
    return count;
}

int8_t q_continuity(const p_queue* q) {
    return q_size(q) == q_count(q);
}

int8_t q_valid(const p_queue* q) {
    return is_empty_queue(q) || (q_continuity(q) && !(q->last == NULL));
}

void q_clear(p_queue* q) {
    if (is_empty_queue(q)) {
        q->size = 0;
        return;
    }
    item* i1,* i2 = q->first;
    q->first = NULL;
    while (i2 != NULL) {
        i1 = i2;
        i2 = i2->next;
        free(i1);
    }
    q->last = NULL;
    q->size = 0;
}