#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int StackPush(struct Stack* stk, StackValueType new_el)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    const int BOOSTCAPASITY = 2;

    if (stk->size > stk->capacity) {
        StackValueType* twin_ptr = (StackValueType*)realloc(stk->data,
                                             (BOOSTCAPASITY * stk->capacity + 2) *
                                              sizeof(StackValueType));

        if (twin_ptr == NULL){
            PRINT_LOGS("Realloc didn`t allocate the memory");
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

int StackPop(struct Stack* stk, StackValueType* last_el)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }
    stk->size--;
    CHECK_STACK(stk);

    *last_el = stk->data[stk->size];

    CHECK_STACK(stk);

    return no_err;
}

int StackCtor(struct Stack* stk, ssize_t stk_size)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }

    stk->capacity = stk_size;

    if (stk->capacity < 0 || stk->capacity > MAXCAPACITY){
        PRINT_LOGS("Incorrect stack capacity");
        return stack_capacity_err;
    }

    StackValueType* twin_data = (StackValueType* )calloc(stk->capacity + 2, sizeof(StackValueType));

    if (twin_data == NULL){
        PRINT_LOGS("Calloc didn`t allocate the memory");
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

void SetStackInfo(Stack* stk, const char* stk_name,
                  const char* file, const char* func,
                  int line)
{
    stk->stack_info.stack_create_file = file;
    stk->stack_info.stack_create_func = func;
    stk->stack_info.stack_create_line = line;
    stk->stack_info.stack_name = stk_name;
}

int StackDtor(struct Stack* stk)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }

    free(stk->data);
    *stk = Stack();
    return no_err;
}
