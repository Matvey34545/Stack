#ifndef DEBUG_STACK
#define DEBUG_STACK

#include <assert.h>

#define __INIT__ Init {__LINE__, __FILE__, __func__}

#define CHECK_STACK(st, init_call)                              \
    if (stack_error(st) != OK)                                  \
    {                                                           \
        dump(stack_err_error(stack_error(st)), st, init_call);  \
        assert(1);                                              \
    }

#endif
