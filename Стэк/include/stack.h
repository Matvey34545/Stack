#ifndef STACK
#define STACK

#include <stdlib.h>
#include "debug_stack.h"

const size_t SIZEBUFFER   = 100;

enum ErrorStack
{
    OK                       = 0,
    OVERFLOW_CAPACITY        = 1,
    OVERFLOW_SIZE            = 2,
    GOING_EDGE               = 3,
    RIGHT_CANARY_DEAD        = 4,
    LEFT_CANARY_DEAD         = 5,
    BUFFER_NULL_PTR          = 6,
    STACK_NULL_PTR           = 7,
    LEFT_BUFFER_CANARY_DEAD  = 8,
    RIGHT_BUFFER_CANARY_DEAD = 9,
    ERROR_ALLOCATION         = 10
};

#ifndef NO_DEBUG
struct Init
{
    int line;
    char *file;
    const char *func;
};
#endif

int create_stack(size_t capacity FOR_DEBUG(, Init init, const char *name));
ErrorStack push_stack(int descriptor, size_t size_element, const void *value FOR_DEBUG(, Init init));
ErrorStack pop_stack(int descriptor, void *value FOR_DEBUG(, Init init));
ErrorStack destroy_stack(int descriptor);


#endif
