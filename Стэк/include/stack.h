#ifndef STACK
#define STACK

#include <stdlib.h>

const size_t SIZEBUFFER = 100; // MINIMUM_BUFFER_SIZE

enum ErrorStack
{
    ERROR_NO         = 0,
    ERROR_ALLOCATION = 1
};

int create_stack(size_t capacity);
ErrorStack push_stack(int descriptor, size_t size_element, const void *value);
ErrorStack pop_stack(int descriptor, void *value);
void destroy_stack(int descriptor);


#endif
