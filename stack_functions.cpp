#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int StackPush(struct Stack* stk, int new_el)
{
    if (stk == NULL){
        fprintf(stdout, "ERROR: NULL stack ptr\n");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    const int BOOSTCAPASITY = 2;

    if (stk->size >= stk->capacity) {
        Stack_t* twin_ptr = (Stack_t*)realloc(stk->data,
                                             (BOOSTCAPASITY * stk->capacity + 1) *
                                              sizeof(int));

        if (twin_ptr == NULL)
            stk->err_info.err_code |= data_ptr_err;

        stk->data = twin_ptr;
    }

    stk->data[stk->size] = new_el;
    stk->size++;

    CHECK_STACK(stk);
    return no_err;
}

int StackPop(struct Stack* stk, int* last_el)
{
    if (stk == NULL){
        fprintf(stdout, "ERROR: NULL stack ptr\n");
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
        fprintf(stdout, "ERROR: NULL stack ptr\n");
        return stack_ptr_err;
    }

    stk->capacity = stk_size;

    if (stk->capacity < 0)
        stk->err_info.err_code |= stack_capacity_err;

    Stack_t* twin_data = (Stack_t* )calloc(stk->capacity, sizeof(int));

    if (twin_data == NULL){
        fprintf(stdout, "ERROR: Calloc allocation \n");
        return data_ptr_err;
    }

    stk->data = twin_data;
    stk->size = 0;

    for (int i = 0;i < stk->capacity; ++i)
        stk->data[i] = POISON;

    CHECK_STACK(stk);

    return no_err;
}

int StackDtor(struct Stack* stk)
{
    if (stk == NULL){
        fprintf(stdout, "ERROR: NULL stack ptr\n");
        return stack_ptr_err;
    }

    free(stk->data);
    *stk = Stack();
    stk = NULL;

    return no_err;
}
