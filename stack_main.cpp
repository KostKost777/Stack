#include <TXLib.h>
#include <stdio.h>

#include "dump_functions.h"
#include "stack_functions.h"

int main()
{
    // FILE* file = fopen("file_with_errors.txt", "w");
    // fclose(file);

    INIT_STACK(stk2);

    ssize_t stk2_size = 10;

    StackCtor(&stk2, stk2_size);

    StackPush(&stk2, 228);
    StackPush(&stk2, 20);
    StackPush(&stk2, 30);

    int last_el = 0;
    StackPop(&stk2, &last_el);

    StackDtor(&stk2);
}
