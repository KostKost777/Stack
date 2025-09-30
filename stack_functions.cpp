#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

extern FILE* log_file;

int StackPush(struct Stack* stk, StackValueType new_el)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    if (stk->size >= stk->capacity + CANARY_CONST) {
        int err = no_err;
        if ((err = GetNewCapacity(stk)) != no_err)
            return err;
    }

    stk->data[stk->size] = new_el;
    stk->size++;

    #ifdef WITHHASH
    stk->stack_info.hash = GetHash(stk);
    #endif

    CHECK_STACK(stk);

    return no_err;
}

int StackPop(struct Stack* stk, StackValueType* last_el)
{
    if (stk == NULL){
        PRINT_LOGS("NULL stack ptr");
        return stack_ptr_err;
    }

    CHECK_STACK(stk);

    stk->size--;

    CHECK_STACK(stk);

    *last_el = stk->data[stk->size];

    stk->data[stk->size] = POISON;

    #ifdef WITHHASH
    stk->stack_info.hash = GetHash(stk);
    #endif

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

    StackValueType* twin_data = (StackValueType* )calloc(stk->capacity + 2,
                                                         sizeof(StackValueType));

    if (twin_data == NULL){
        PRINT_LOGS("Calloc didn`t allocate the memory");
        return data_ptr_err;
    }

    stk->data = twin_data;

    #ifndef WITHCANARY

    stk->size = 0;

    #else

    stk->size = 1;
    stk->data[0] = CANARY;
    stk->data[stk->capacity + 1] = CANARY;

    #endif

    SetPoisonValues(stk);

    #ifdef WITHHASH
    stk->stack_info.hash = GetHash(stk);
    #endif

    CHECK_STACK(stk);

    return no_err;
}

void PrintLogs(const char* file, const char* func,
               int line, const char* err_description)
{
    fprintf(log_file, "ERROR: %s in %s %s:%d \n", err_description,
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


int GetNewCapacity(Stack* stk)
{
    const int BOOSTCAPASITY = 2;

    int new_capacity = BOOSTCAPASITY * stk->capacity;

    StackValueType* twin_ptr = (StackValueType*)realloc(stk->data,
                                                        (new_capacity + 2) *
                                                        sizeof(StackValueType));
    if (twin_ptr == NULL){
        PRINT_LOGS("Realloc didn`t allocate the memory");
        return data_ptr_err;
    }

    stk->data = twin_ptr;

    #ifdef WITHCANARY

    stk->data[stk->capacity + 1] = POISON;
    stk->data[new_capacity + 1] = CANARY;

    #endif

    for (int i = stk->capacity + CANARY_CONST; i < new_capacity + CANARY_CONST; ++i) {
        stk->data[i] = POISON;
    }

    stk->capacity = new_capacity;

    return no_err;
}

void SetPoisonValues(Stack* stk)
{
    int start_index = CANARY_CONST;
    int end_index = stk->capacity + CANARY_CONST;

    for (int i = start_index; i < end_index; ++i)
        stk->data[i] = POISON;
}
