#include "stack.h"
#include "debug_stack.h"

#include <stdio.h>

int main()
{
    int value = 2;
    int value1 = 2;

    int descriptor = create_stack(SIZEBUFFER FOR_DEBUG(, __INIT__, "descriptor"));
    int descriptor2 = create_stack(SIZEBUFFER FOR_DEBUG(, __INIT__, "descriptor"));

    for (int i = 0; i < 6; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor, sizeof(int), &value FOR_DEBUG(, __INIT__));
    }
    for (int i = 0; i < 6; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor2, sizeof(int), &value FOR_DEBUG(, __INIT__));
    }
    printf("*********************\n");
    for (int i = 0; i < 6; i++)
    {
        pop_stack(descriptor, sizeof(int), &value FOR_DEBUG(, __INIT__));
        printf("Stack1: %d\n\n", value);
    }
    for (int i = 0; i < 6; i++)
    {
        pop_stack(descriptor2, sizeof(int), &value FOR_DEBUG(, __INIT__));
        printf("Stack2: %d\n\n", value);
    }
    destroy_stack(descriptor);
    destroy_stack(descriptor2);
}

