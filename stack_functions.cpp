#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int StackPush(struct Stack* stk, int new_el)
{
    int err_code = 0;
    CHECK_STACK(stk);

    const int BOOSTCAPASITY = 2;

    if (stk->size >= stk->capacity) {
        Stack_t* twin_ptr = (Stack_t*)realloc(stk->data,
                                             (BOOSTCAPASITY * stk->capacity + 1) *
                                              sizeof(int));

        if (twin_ptr == NULL){
            StackDump(stk, stack_capacity_err);
            return  stack_capacity_err;
        }

        stk->data = twin_ptr;
    }

    stk->data[stk->size] = new_el;
    stk->size++;

    CHECK_STACK(stk);
    return no_err;
}

int StackPop(struct Stack* stk, int* last_el)
{
    int err_code = 0;
    CHECK_STACK(stk);

    *last_el = stk->data[--stk->size];

    CHECK_STACK(stk);

    return no_err;
}

int StackCtor(struct Stack* stk, ssize_t stk_size)
{
    if (stk == NULL){
        StackDump(stk, stack_ptr_err);
        return stack_ptr_err;
    }

    int err_code = 0;

    stk->capacity = stk_size;

    if (stk->capacity < 0)
        err_code |= stack_capacity_err;


    Stack_t* twin_data = (Stack_t* )calloc(stk->capacity, sizeof(int));

    if (twin_data == NULL)
        err_code |= data_ptr_err;

    stk->data = twin_data;
    stk->size = 0;

    if (err_code != no_err){
        StackDump(stk, err_code);
        return err_code;
    }

    return no_err;
}

int StackDtor(struct Stack* stk)
{
    if (stk == NULL){
        StackDump(stk, stack_ptr_err);
        return stack_ptr_err;
    }

    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;
    stk = NULL;
    return no_err;
}
