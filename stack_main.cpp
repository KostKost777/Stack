#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int main()
{
    FILE* file = fopen(log_file_name, "w");
    fclose(file);

    INIT_STACK(stk2);

    ssize_t stk2_size = 10;

    StackCtor(&stk2, stk2_size);

    StackPush(&stk2, 10);
    StackPush(&stk2, 20);
    stk2.data[0] = 5;
    StackPush(&stk2, 30);

    int last_el = 0;
    StackPop(&stk2, &last_el);

    StackDtor(&stk2);
}
