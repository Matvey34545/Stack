#include "stack.h"
#include <stdio.h>

int main()
{
    int value = 2;
    char x ='\0';

    int descriptor = create_stack(SIZEBUFFER);

    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &value);
        push_stack(descriptor, sizeof(int), &value);
    }
    for (int i = 0; i < 8; i++)
    {
        scanf("%c", &x);
        push_stack(descriptor, sizeof(int), &x);
    }
    printf("*********************\n");
    for (int i = 0; i < 8; i++)
    {
        pop_stack(descriptor, &x);
        printf("Stack: %c\n\n", x);
    }
    for (int i = 0; i < 8; i++)
    {
        pop_stack(descriptor, &value);
        printf("Stack: %d\n\n", value);
    }
}

