#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"
#include "calc_operations.h"

#define NCANARY

const int MAXLEN = 10;

int main()
{
    INIT_STACK(stk2);

    ssize_t stk2_size = 1;

    StackCtor(&stk2, stk2_size);

    char operation[MAXLEN] = "\0";

    while (true){
        scanf("%s", operation);

        if (!strcmp(operation, "HLT"))
            return 0;

        ChooseCommand(&stk2, operation);
        StackDump(&stk2);
    }

    StackDtor(&stk2);
}
