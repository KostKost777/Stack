#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

int StackVerifier(struct Stack* stk)
{
    int err_code = 0;

    if (stk == NULL)
        err_code ^= stack_ptr_err;

    if (stk->data == NULL)
        err_code ^= data_ptr_err;

    if (stk->capacity <= 0 || stk->capacity > MAXCAPACITY)
        err_code ^= stack_capacity_err;

    if (stk->size < 0 || stk->size > MAXSIZE)
        err_code ^= stack_size_err;

    return err_code;
}

void PrintErrors(int err_code)
{
    if (err_code & stack_ptr_err)
        printf("BADSTACKPTR(%d) ", stack_ptr_err);

    if (err_code & data_ptr_err)
        printf("BADDATAPTR(%d) ", data_ptr_err);

    if (err_code & stack_capacity_err)
        printf("BADCAPACITY(%d) ", stack_capacity_err);

    if (err_code & stack_size_err)
        printf("BADSIZE(%d) ", stack_size_err);
}

void StackDump(struct Stack* stk, int err_code)
{
    printf("StackDump()\n"
           "stack[%x] ",
           (unsigned int)stk);

    PrintErrors(err_code);

    //стурктура исправна
    if (!(err_code & stack_ptr_err)) {
        printf("\n{");

        printf("\n    size = %d", stk->size);
        //индекс последнего элемента не корректный
        if (err_code & stack_size_err)
            printf("    (BADSIZE!)");

        printf("\n    capacity = %d", stk->capacity);
        //размер массива не корректныый
        if (err_code & stack_capacity_err)
            printf("    (BADCAPACITY!)");
    }

    printf("\n    data[%x] ", (unsigned int)stk->data);
    //нулевой указатель на массив
    if (err_code & data_ptr_err)
        printf("    (BADDATAPTR!)");
    printf("\n");

    //сруктура и массив исправны
    if (!(err_code & stack_ptr_err) && !(err_code & data_ptr_err)){

        // индекс последнего элемента не корректный, но размер массива корректный
        if ((err_code & stack_size_err) && !(err_code & stack_capacity_err))
            PrintDataWithCorrectCapacity(stk);

        // индекс последнего элемента корректный, но размер массива не корректный
        if (!(err_code & stack_size_err) && (err_code & stack_capacity_err))
            PrintDataWithCorrectSize(stk);

        // индекс последнего элемента корректный, но размер массива не корректный
        if ((err_code & stack_size_err) && (err_code & stack_capacity_err))
            PrintDataWithUnknownParam(stk);
    }

    printf("}");
}

void PrintDataWithCorrectCapacity(struct Stack* stk) {
    for (int i = 0; i < stk->capacity; ++i) {
        printf("    [%d] - %d   (UNKNOWNSIZE!)\n", i, stk->data[i]);
    }
}

void PrintDataWithCorrectSize(struct Stack* stk){
    for (int i = 0; i < stk->size; ++i) {
        printf("    *[%d] - %d   (UNKNOWNCAPACITY!)\n", i, stk->data[i]);
    }
}

void PrintDataWithUnknownParam(struct Stack* stk){
    const int MAXVALUE = 5;
    for (int i = 0; i < MAXVALUE; ++i) {
        printf("    [%d] - %d   (UNKNOWNPARAMS!)\n", i, stk->data[i]);
    }
}
