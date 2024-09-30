#include "stack.h"
#include <stdio.h>

int main()
{
    int value = 2;
    int value1 = 2;

    int descriptor = create_stack(SIZEBUFFER);
    int descriptor1 = create_stack(SIZEBUFFER);

    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor, sizeof(int), &value);
    }
    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor1, sizeof(int), &value);
    }
    printf("*********************\n");
    for (int i = 0; i < 8; i++)
    {
        pop_stack(descriptor, &value);
        printf("Stack: %d\n\n", value);
    }
    for (int i = 0; i < 8; i++)
    {
        pop_stack(descriptor1, &value);
        printf("Stack: %d\n\n", value);
    }
    destroy_stack(descriptor);
    destroy_stack(descriptor1);
}

