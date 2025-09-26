#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int StackPush(struct Stack* stk, int new_el)
{
    if (stk == NULL){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "NULL stack ptr");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    const int BOOSTCAPASITY = 2;

    if (stk->size > stk->capacity) {
        Stack_t* twin_ptr = (Stack_t*)realloc(stk->data,
                                             (BOOSTCAPASITY * stk->capacity + 2) *
                                              sizeof(int));

        if (twin_ptr == NULL){
            PrintLogs(__FILE__, __func__, __LINE__,
                 "Realloc didn`t allocate the memory");
            return data_ptr_err;
        }

        stk->data = twin_ptr;
    }



    stk->data[stk->capacity + 1] = POISON;

    stk->capacity = BOOSTCAPASITY * stk->capacity + 2;

    stk->data[stk->capacity + 1] = CANARY;

    stk->data[stk->size] = new_el;
    stk->size++;

    CHECK_STACK(stk);
    return no_err;
}

int StackPop(struct Stack* stk, int* last_el)
{
    if (stk == NULL){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "NULL stack ptr");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    *last_el = stk->data[--stk->size];

    CHECK_STACK(stk);

    return no_err;
}

int StackCtor(struct Stack* stk, ssize_t stk_size)
{
    if (stk == NULL){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "NULL stack ptr");
        return stack_ptr_err;
    }

    stk->capacity = stk_size;

    if (stk->capacity < 0 || stk->capacity > MAXCAPACITY){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "Incorrect stack capacity");
        return stack_capacity_err;
    }

    Stack_t* twin_data = (Stack_t* )calloc(stk->capacity + 2, sizeof(int));

    if (twin_data == NULL){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "Calloc didn`t allocate the memory");
        return data_ptr_err;
    }

    stk->data = twin_data;
    stk->size = 1;

    stk->data[0] = CANARY;
    stk->data[stk->capacity + 1] = CANARY;

    for (int i = 1;i <= stk->capacity; ++i)
        stk->data[i] = POISON;

    CHECK_STACK(stk);

    return no_err;
}

void PrintLogs(const char* file, const char* func,
               int line, const char* err_description)
{
    fprintf(stdout, "ERROR: %s in %s %s:%d \n", err_description,
                                                func, file, line);
}

int StackDtor(struct Stack* stk)
{
    if (stk == NULL){
        PrintLogs(__FILE__, __func__, __LINE__,
                 "NULL stack ptr\n");
        return stack_ptr_err;
    }

    free(stk->data);
    *stk = Stack();
    stk = NULL;

    return no_err;
}
