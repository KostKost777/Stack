#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int main()
{
    Stack stk1 = {};
    ssize_t stk1_size = 5;

    CHECK_KOSTIK ( StackCtor(&stk1, stk1_size) );

    CHECK_KOSTIK ( StackPush(&stk1, 10) );
    CHECK_KOSTIK ( StackPush(&stk1, 20) );
    CHECK_KOSTIK ( StackPush(&stk1, 30) );

    stk1.capacity = -10;

    int last_el = 0;
    CHECK_KOSTIK ( StackPop(&stk1, &last_el) );

    printf("last_el = %d (A Quostik ne pishett NAZWANNIYAAA peremennyh!!!\n", last_el);

    StackDtor(&stk1);
}
