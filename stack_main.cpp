#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int main()
{
    INIT_STACK(stk2);

    ssize_t stk2_size = 5;

    CHECK_ERROR (StackCtor(&stk2, stk2_size), stk2);

    CHECK_ERROR (StackPush(&stk2, 10), stk2);
    CHECK_ERROR (StackPush(&stk2, 20), stk2);
    CHECK_ERROR (StackPush(&stk2, 30), stk2);

    StackDump(&stk2);
    int last_el = 0;

    CHECK_ERROR (StackPop(&stk2, &last_el), stk2);

    printf("last_el = %d \n", last_el);

    StackDtor(&stk2);
}
