#ifndef DEBUG_STACK
#define DEBUG_STACK

#include <assert.h>


#ifdef NO_DEBUG
#define    FOR_DEBUG(...)

#else
#define    FOR_DEBUG(...) __VA_ARGS__



#define __INIT__ Init {__LINE__, __FILE__, __func__}

#define CHECK_STACK(st, init_call)                              \
    if (stack_error(st) != OK)                                  \
    {                                                           \
        dump(stack_err_error(stack_error(st)), st, init_call);  \
        assert(1);                                              \
    }
#endif

#endif
