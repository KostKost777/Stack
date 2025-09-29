#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"
#include "calc_operations.h"

void ChooseCommand(Stack* stk, char* operation)
{
    if (!strcmp(operation, "PUSH"))
        Push(stk);

    if (!strcmp(operation, "ADD"))
        Add(stk);

    if (!strcmp(operation, "SUB"))
        Sub(stk);

    if (!strcmp(operation, "MUL"))
        Mul(stk);

    if (!strcmp(operation, "DIV"))
        Div(stk);

    if (!strcmp(operation, "OUT"))
        Out(stk);
}

void Push(Stack* stk)
{
    StackValueType new_value = 0;
    scanf("%d", &new_value);
    StackPush(stk, new_value);
}

void Add(Stack* stk)
{
    StackValueType value1 = 0;
    StackValueType value2 = 0;

    StackPop(stk, &value1);
    StackPop(stk, &value2);

    StackPush(stk, value1 + value2);
}

void Sub(Stack* stk)
{
    StackValueType value1 = 0;
    StackValueType value2 = 0;

    StackPop(stk, &value1);
    StackPop(stk, &value2);

    StackPush(stk, value2 - value1);
}

void Mul(Stack* stk)
{
    StackValueType value1 = 0;
    StackValueType value2 = 0;

    StackPop(stk, &value1);
    StackPop(stk, &value2);

    StackPush(stk, value1 * value2);
}

void Div(Stack* stk)
{
    StackValueType value1 = 0;
    StackValueType value2 = 0;

    StackPop(stk, &value1);
    StackPop(stk, &value2);

    if (value1 == 0)
        printf("You can`t divide by zero\n");
    else
        StackPush(stk, value2 / value1);
}

void Out(Stack* stk)
{
    StackValueType value2 = 0;

    StackPop(stk, &value2);

    printf("%d", value2);
}



