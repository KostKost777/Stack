#include <TXLib.h>
#include <stdio.h>

#include "stack.h"

#define CHECK_STACK(stack_ptr)       \
int err_code = StackVerifier(stk);   \
    if (err_code != no_err){         \
        StackDump(stk, err_code);    \
        return err_code;             \
    }                                \


int main()
{
    Stack stk1 = {};
    ssize_t stk1_size = 5;

    if (StackCtor(&stk1, stk1_size) != no_err)
        return 0;


    if (StackPush(&stk1, 1000000000000000000000000) != no_err)
        return 0;

//     if (StackPush(&stk1, 20) != no_err)
//         return 0;
//
//     if (StackPush(&stk1, 30) != no_err)
//         return 0;

    int last_el = 0;
   // stk1.capacity = 10;

    if (StackPop(&stk1, &last_el) != no_err)
        return 0;
    printf("%d\n", last_el);

    StackDtor(&stk1);
}

int StackPush(struct Stack* stk, int new_el)
{
    CHECK_STACK(stk);

    const int BOOSTCAPASITY = 2;

    if (stk->size >= stk->capacity) {
        Stack_t* twin_ptr = (Stack_t* )realloc(stk->data,
                                               BOOSTCAPASITY * stk->capacity + 1);

        if (twin_ptr == NULL){
            err_code = stack_capacity_err;
            StackDump(stk, stack_capacity_err);
            return  stack_capacity_err;
        }

        stk->data = twin_ptr;
    }

    stk->data[stk->size] = new_el;
    stk->size++;

    return no_err;
}

int StackPop(struct Stack* stk, int* last_el)
{
    CHECK_STACK(stk);

    *last_el = stk->data[--stk->size];
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
        err_code ^= stack_capacity_err;


    Stack_t* twin_data = (Stack_t* )calloc(stk->capacity, sizeof(int));

    if (twin_data == NULL)
        err_code ^= data_ptr_err;

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
    free(stk->data);
    stk->data = NULL;
    return no_err;
}

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

        // размер массива и индекс последнего элемента корректны
        if (!(err_code & stack_size_err) && !(err_code & stack_capacity_err))
            PrintDataWithAllCorrect(stk);

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

void PrintDataWithAllCorrect(struct Stack* stk){
    for (int i = 0; i < stk->capacity; ++i) {

        printf("    ");
        if (i < stk->size)
            printf("*");

        printf("[%d] - %d", i, stk->data[i]);

        if (i >= stk->size)
            printf("    (POISON!)");

        printf("\n");
    }
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





