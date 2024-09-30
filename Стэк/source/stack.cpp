#include "stack.h"
#include "bynar_tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct stack_t
{
    int descriptor;
    size_t capacity;
    size_t size;
    void* data;
};

static BynarTree tree = {};

static ErrorStack realocation_st(stack_t *st);
static int comparison_desc(const void *a, const void *b);

int create_stack(size_t capacity)
{
    static bool struct_descrip_is_ready = false;
    if (!struct_descrip_is_ready)
    {
        create_bynar_tree(&tree, sizeof(stack_t));
        struct_descrip_is_ready = true;
    }

    srand(time(NULL));
    int descriptor = 0;
    void *ptr = NULL;
    do
    {
        descriptor = rand();
        ptr = find_elem(&tree, &descriptor, comparison_desc);
    } while (ptr != NULL);

    stack_t st = {};
    st.data = calloc(capacity, sizeof(char));
    st.capacity = capacity;
    st.size = 0;
    st.descriptor = descriptor;

    insert_top(&tree, &st, comparison_desc);

    return descriptor;
}

ErrorStack push_stack(int descriptor, size_t size_element, const void *value)
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_desc);
    st->size += size_element + sizeof(size_t);
    ErrorStack error = realocation_st(st);
    if (error!= ERROR_NO)
        return error;

    memcpy(st->data + st->size - size_element - sizeof(size_t), value, size_element);
    memcpy(st->data + st->size - sizeof(size_t), &size_element, sizeof(size_t));
    return ERROR_NO;
}

ErrorStack pop_stack(int descriptor, void *value)
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_desc);
    size_t size_element = *(size_t*)(st->data + st->size - sizeof(size_t));
    st->size -= size_element + sizeof(size_t);
    memcpy(value, st->data + st->size, size_element);

    ErrorStack error = realocation_st(st);
    return error;
}

static ErrorStack realocation_st(stack_t *st)
{
    if (st->capacity <= st->size)
    {
        void *temp = NULL;
        temp = realloc(st->data, 2 * st->capacity);
        if (temp == NULL)
            return ERROR_ALLOCATION;

        st->capacity *= 2;
        st->data = temp;
    }
    if (st->capacity >= 4 * st->size && st->capacity > SIZEBUFFER)
    {
        void *temp = NULL;
        temp = realloc(st->data, st->capacity / 2);
        if (temp == NULL)
            return ERROR_ALLOCATION;

        st->capacity /= 2;
        st->data = temp;
    }
    return ERROR_NO;
}

static int comparison_desc(const void *a, const void *b)
{
    stack_t* st1 = (stack_t*)a;
    stack_t* st2 = (stack_t*)b;
    return st1->descriptor - st2->descriptor;
}

void destroy_stack(int descriptor)
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_desc);
    free(st->data);
    delete_elem(&tree, &descriptor, comparison_desc);
}



