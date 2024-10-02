#include "stack.h"
#include "bynar_tree.h"
#include "debug_stack.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

const char* FILENAME = "log.txt";
const unsigned int CANARY = 0xDED0BEDA;
const size_t MAX_SIZE     = 0xFFFFFFFFFFFFAAAA;

#ifndef NO_DEBUG

struct StDebug
{
    int born_line;
    char *born_file;
    char *born_func;
    const char *name;
};

#endif

struct stack_t
{
    FOR_DEBUG(const unsigned int canary_left;)
    int descriptor;
    size_t capacity;
    size_t size;
    void* data;
    FOR_DEBUG(StDebug debug;)
    FOR_DEBUG(const unsigned int canary_right;)
};

static BynarTree tree = {};

static ErrorStack realocation_st(stack_t *st);
static int comparison_desc(const void *a, const void *b);
static int comparison_with_descr(const void *descr, const void *st);
static ErrorStack stack_error(stack_t *st);
static void dump(const char *error, stack_t *st FOR_DEBUG(,Init init));
static const char* stack_err_error(ErrorStack error);

int create_stack(size_t capacity FOR_DEBUG(,Init init, const char* name))
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
        ptr = find_elem(&tree, &descriptor, comparison_with_descr);
    } while (ptr != NULL);

    stack_t st = {FOR_DEBUG(CANARY,) descriptor, capacity, 0, NULL
                  FOR_DEBUG(,{init.line, init.file, (char*)init.func, name}, CANARY)};

    st.data = calloc(capacity + 2 * sizeof(unsigned int), sizeof(char));
    if (st.data == NULL)
        return -1;

    #ifndef NO_DEBUG
    memcpy(st.data, &CANARY, sizeof(CANARY));
    memcpy(st.data + capacity + sizeof(CANARY), &CANARY, sizeof(CANARY));
    st.data = st.data + sizeof(CANARY);
    #endif

    ErrorBynarTree error = insert_top(&tree, &st, comparison_desc);
    if (error != BYNAR_OK)
        return - 2;

    return descriptor;
}

ErrorStack push_stack(int descriptor, size_t size_element, const void *value FOR_DEBUG(, Init init))
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_with_descr);
    FOR_DEBUG(CHECK_STACK(st, init))
    st->size += size_element + sizeof(size_t);
    ErrorStack error = realocation_st(st);
    if (error!= OK)
        return error;

    memcpy(st->data + st->size - size_element - sizeof(size_t), value, size_element);
    memcpy(st->data + st->size - sizeof(size_t), &size_element, sizeof(size_t));
    FOR_DEBUG(CHECK_STACK(st, init))
    return OK;
}

ErrorStack pop_stack(int descriptor, void *value FOR_DEBUG(, Init init))
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_with_descr);
    FOR_DEBUG(CHECK_STACK(st, init))
    size_t size_element = *(size_t*)(st->data + st->size - sizeof(size_t));
    st->size -= size_element + sizeof(size_t);
    memcpy(value, st->data + st->size, size_element);

    ErrorStack error = realocation_st(st);
    if (error!= OK)
        return error;

    FOR_DEBUG(CHECK_STACK(st, init))
    return OK;
}

static ErrorStack realocation_st(stack_t *st)
{
    if (st->capacity <= st->size)
    {
        void *temp = NULL;
        temp = realloc(st->data FOR_DEBUG(- sizeof(CANARY)), 2 * st->capacity FOR_DEBUG(+ 2 * sizeof(CANARY)));
        if (temp == NULL)
            return ERROR_ALLOCATION;

        st->capacity *= 2;
        st->data = temp FOR_DEBUG(+ sizeof(CANARY));
    }
    if (st->capacity >= 4 * st->size && st->capacity > SIZEBUFFER)
    {
        void *temp = NULL;
        temp = realloc(st->data FOR_DEBUG(- sizeof(CANARY)), st->capacity / 2 FOR_DEBUG(+ 2 * sizeof(CANARY)));
        if (temp == NULL)
            return ERROR_ALLOCATION;

        st->capacity /= 2;
        st->data = temp FOR_DEBUG(+ sizeof(CANARY));
    }
    FOR_DEBUG(memcpy(st->data + st->capacity, &CANARY, sizeof(CANARY));)
    return OK;
}

static int comparison_desc(const void *a, const void *b)
{
    stack_t* st1 = (stack_t*)a;
    stack_t* st2 = (stack_t*)b;
    return st1->descriptor - st2->descriptor;
}

static int comparison_with_descr(const void *descr, const void *st)
{
    stack_t* st1 = (stack_t*)st;
    return *(int*)descr - st1->descriptor;
}


ErrorStack destroy_stack(int descriptor)
{
    stack_t *st = (stack_t*)find_elem(&tree, &descriptor, comparison_with_descr);
    if (st == NULL)
        return STACK_NULL_PTR;

    if (st->data == NULL)
        return BUFFER_NULL_PTR;

    free(st->data FOR_DEBUG(- sizeof(CANARY)));
    delete_elem(&tree, &descriptor, comparison_desc);
    return OK;
}

#ifndef NO_DEBUG

static ErrorStack stack_error(stack_t *st)
{
    if (st == NULL)
        return STACK_NULL_PTR;

    if (st->data == NULL)
        return BUFFER_NULL_PTR;

    if (st->canary_left != CANARY)
        return LEFT_CANARY_DEAD;

    if (st->canary_right != CANARY)
        return RIGHT_CANARY_DEAD;

    if (st->size >= st->capacity)
        return GOING_EDGE;

    if (st->size >= MAX_SIZE)
        return OVERFLOW_SIZE;

    if (st->capacity >= MAX_SIZE)
        return OVERFLOW_CAPACITY;

    if (*(unsigned int*)(st->data - sizeof(CANARY)) != CANARY)
        return LEFT_BUFFER_CANARY_DEAD;

    if (*(unsigned int*)(st->data + st->capacity) != CANARY)
        return RIGHT_BUFFER_CANARY_DEAD;

    return OK;
}

static void dump(const char *error, stack_t *st, Init init)
{
    FILE *fp = fopen(FILENAME, "a");

    fprintf(fp, "%s [%p] at %s:%d (%s) born %s:%d (%s)\n\t\n\t", (st->debug).name, st, init.file, init.line, init.func,
            (st->debug).born_file, (st->debug).born_line, (st->debug).born_func);

    fprintf(fp, "size = %d\n\tcapacity = %d\n\tdata[%p]\n\tERROR: %s\n\t", st->size, st->capacity, st->data, error);
    fprintf(fp, "LEFT_CANARY = %u\n\tRIGHT_CANARY = %u\n\tLEFT_BUFFER_CANARY = %u\n\tRIGHT_BUFFER_CANARY = %u\n\n\tvalue elements:\n\t",
                st->canary_left, st->canary_right, *(unsigned int*)(st->data - sizeof(CANARY)), *(unsigned int*)(st->data + st->capacity));

    for (int i = 0; i < st->size; i++)
        fprintf(fp, "%x\n\t", *((uint8_t*)st->data + i));

    for (int i = st->size; i < st->capacity; i++)
        fprintf(fp, "*%x (POISON)\n\t", *((uint8_t*)st->data + i));

    fprintf(fp, "}\n");

    fclose(fp);
}

static const char* stack_err_error(ErrorStack error)
{
    #define DESCR_(e)     \
        case e: return #e;\
            break

    switch(error)
    {
        DESCR_(STACK_NULL_PTR);
        DESCR_(BUFFER_NULL_PTR);
        DESCR_(LEFT_CANARY_DEAD);
        DESCR_(RIGHT_CANARY_DEAD);
        DESCR_(GOING_EDGE);
        DESCR_(OVERFLOW_SIZE);
        DESCR_(OVERFLOW_CAPACITY);
        DESCR_(LEFT_BUFFER_CANARY_DEAD);
        DESCR_(RIGHT_BUFFER_CANARY_DEAD);
        DESCR_(OK);
    }
    return "OK";
    #undef DESCR_
}

#endif










