#ifndef _DA_
#define _DA_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memmove

typedef struct _DA_Header
{
    size_t length;
    size_t capacity;
    size_t item_size;
} _DA_Header;

extern void *da_init(const size_t item_size, const size_t capacity);
extern void da_cleanup(void *da);

extern void da_ensure_capacity(void **da, const size_t capacity_increase);

extern size_t da_priority_insert(void **da, const size_t priority,
                                 int (*compare)(const void *, const size_t));
extern size_t da_insert_at(void **da, size_t index);

extern void da_pop_start(void *da);
extern void da_pop_end(void *da);

extern void da_reverse(void *da);

extern size_t da_length(const void *da);
extern void da_increment_length(void *da);

#endif
