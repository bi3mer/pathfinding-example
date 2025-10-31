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

static void *da_init(const size_t item_size, const size_t capacity)
{
    void *ptr = 0;
    _DA_Header *h =
        (_DA_Header *)malloc(item_size * capacity + sizeof(_DA_Header));

    if (h)
    {
        h->length = 0;
        h->capacity = capacity;
        h->item_size = item_size;
        ptr = h + 1;
    }
    else
    {
        fprintf(stderr, "Unable to initialize dynamic array with malloc.\n");
        exit(1);
    }

    return ptr;
}

static void da_ensure_capacity(void **da, const size_t capacity_increase)
{
    _DA_Header *h = ((_DA_Header *)(*da) - 1);
    if (h->length + capacity_increase > h->capacity)
    {
        size_t new_capacity = h->capacity * 2;
        while (h->length + capacity_increase > new_capacity)
        {
            new_capacity *= 2;
        }

        h = (_DA_Header *)realloc(h, h->item_size * new_capacity +
                                         sizeof(_DA_Header));
        if (!h)
        {
            fprintf(stderr, "Unable to resize dynamic array with realloc.\n");
            exit(1);
        }

        h->capacity = new_capacity;
        *da = h + 1;
    }
}

static size_t da_priority_insert(void **da, const size_t priority,
                                 int (*compare)(const void *, const size_t))
{
    size_t i, insert_index;

    da_ensure_capacity(da, 1);
    _DA_Header *h = ((_DA_Header *)(*da) - 1);
    char *bytes = (char *)(*da);

    insert_index = h->length;

    for (i = 0; i < h->length; ++i)
    {
        if (compare(bytes + (i * h->item_size), priority) > 0)
        {
            insert_index = i;
            break;
        }
    }

    if (insert_index < h->length)
    {
        memmove(bytes + ((insert_index + 1) * h->item_size),
                bytes + (insert_index * h->item_size),
                (h->length - insert_index) * h->item_size);
    }

    ++h->length;

    return insert_index;
}

static size_t da_insert_at(void **da, size_t index)
{
    da_ensure_capacity(da, 1);
    _DA_Header *h = ((_DA_Header *)(*da) - 1);
    char *bytes = (char *)(*da);

    if (index > h->length) // not sure I like this choice...
    {
        index = h->length;
    }

    if (index < h->length)
    {
        memmove(bytes + ((index + 1) * h->item_size),
                bytes + (index * h->item_size),
                (h->length - index) * h->item_size);
    }

    ++h->length;
    return index;
}

static void da_pop_start(void *da)
{
    if (!da)
        return;

    _DA_Header *h = ((_DA_Header *)da - 1);
    if (h->length == 0)
        return;

    char *bytes = (char *)da;

    if (h->length > 1)
    {
        memmove(bytes, bytes + h->item_size, (h->length - 1) * h->item_size);
    }

    h->length--;
}

static void da_pop_end(void *da)
{
    if (!da)
        return;

    _DA_Header *h = ((_DA_Header *)da - 1);
    if (h->length > 0)
    {
        h->length--;
    }
}

static inline size_t da_length(const void *da)
{
    return da ? ((const _DA_Header *)da - 1)->length : 0;
}

static inline void da_increment_length(void *da)
{
    if (da)
    {
        ((_DA_Header *)(da)-1)->length++;
    }
}

static inline void da_free(void *da)
{
    if (da)
    {
        free(((_DA_Header *)(da)-1));
    }
}

#endif
