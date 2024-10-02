#ifndef DEBUG_STACK
#define DEBUG_STACK

#include <assert.h>


#ifdef NO_DEBUG
    #define    FOR_DEBUG(...)
    #define CHECK_STACK(st, init)                                          \
    if (stack_error(st) != OK)                                             \
        assert(0);

#else
    #define    FOR_DEBUG(...) __VA_ARGS__
    #define __INIT__ Init {__LINE__, __FILE__, __func__}

    #define CHECK_STACK(st, init_call)                                     \
    if (stack_error(st) != OK)                                             \
    {                                                                      \
        dump(stack_err_error(stack_error(st)), st, init_call);             \
        assert(0);                                                         \
    }

#endif

#ifdef NO_CANARY
    #define FOR_CANARY(...)

#else
    #define FOR_CANARY(...) __VA_ARGS__

#endif

#endif
