#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int main()
{
    Stack stk1 = {};
    ssize_t stk1_size = 5;

    if (StackCtor(&stk1, stk1_size) != no_err)
        return 0;

    if (StackPush(&stk1, 10) != no_err)
        return 0;

    if (StackPush(&stk1, 20) != no_err)
        return 0;

    if (StackPush(&stk1, 30) != no_err)
        return 0;

    int last_el = 0;
    stk1.capacity = -10;

    if (StackPop(&stk1, &last_el) != no_err)
        return 0;
    printf("%d\n", last_el);

    StackDtor(&stk1);
}
