#include "stack.h"
#include "debug_stack.h"

#include <stdio.h>

int main()
{
    int value = 2;
    int value1 = 2;
    printf("%d\n", sizeof(int*));
    int descriptor = create_stack(SIZEBUFFER FOR_DEBUG(, __INIT__, "descriptor"));



    for (int i = 0; i < 12; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor, sizeof(int), &value FOR_DEBUG(, __INIT__));
    }
    printf("*********************\n");
    for (int i = 0; i < 12; i++)
    {
        pop_stack(descriptor, &value FOR_DEBUG(, __INIT__));
        printf("Stack: %d\n\n", value);
    }
    destroy_stack(descriptor);
}

