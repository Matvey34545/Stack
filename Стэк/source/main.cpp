#include "stack.h"
#include "debug_stack.h"

#include <stdio.h>

int main()
{
    int value = 2;
    int value1 = 2;

    int descriptor = create_stack(SIZEBUFFER, __INIT__, "descriptor");

    for (int i = 0; i < 12; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor, sizeof(int), &value, __INIT__);
    }
    printf("*********************\n");
    for (int i = 0; i < 12; i++)
    {
        pop_stack(descriptor, &value, __INIT__);
        printf("Stack: %d\n\n", value);
    }
    destroy_stack(descriptor);
}

