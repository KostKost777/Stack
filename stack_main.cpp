#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

FILE* log_file = fopen("logfile.txt", "w");


int main()
{

    INIT_STACK(stk2);

    ssize_t stk2_size = 10;

    StackCtor(&stk2, stk2_size);

    StackPush(&stk2, 10);
    StackPush(&stk2, 20);
    StackPush(&stk2, 30);

    int last_el = 0;
    StackPop(&stk2, &last_el);

    StackDump(&stk2);

    StackDtor(&stk2);
    fclose(log_file);
}
