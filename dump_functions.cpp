#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

void SetErrorInfo(Stack* stk,
                  const char* file, const char* func, int line)
{
    stk->err_info.err_file = file;
    stk->err_info.err_func = func;
    stk->err_info.err_line = line;
}

void SetIndexes(Stack* stk, int* s_i, int* e_i)
{
    #ifdef NCANARY
    *s_i = 0;
    *e_i = stk->capacity;

    #else
    *s_i = 1;
    *e_i = stk->capacity + 1;

    #endif

}


int StackVerifier(struct Stack* stk)
{
    if (stk == NULL)
        stk->err_info.err_code |= stack_ptr_err;

    if (stk->data == NULL)
        stk->err_info.err_code |= data_ptr_err;

    if (stk->capacity <= 0 || stk->capacity >= MAXCAPACITY)
        stk->err_info.err_code |= stack_capacity_err;

    #ifndef NCANARY

    if (stk->data != NULL && stk->capacity > 0 && stk->capacity < MAXCAPACITY) {

        if (stk->data[0] != CANARY)
            stk->err_info.err_code |= begin_canary_err;

        if (stk->data[stk->capacity + 1] != CANARY)
            stk->err_info.err_code |= end_canary_err;
    }
    #endif

    if (stk->size < 1 || stk->size >= MAXSIZE)
        stk->err_info.err_code |= stack_size_err;

    else {
        if (stk->size > 1)
            if (stk->data[stk->size - 1] == POISON)
                stk->err_info.err_code |= poison_element_err;
    }

    return stk->err_info.err_code;
}

void PrintErrors(struct Stack* stk)
{


    fprintf(stdout, "ERROR in %s %s:%d  \n", stk->err_info.err_func,
                                             stk->err_info.err_file,
                                             stk->err_info.err_line);

    if (stk->err_info.err_code & stack_ptr_err)
        fprintf(stdout, "BADSTACKPTR(%d) ", stack_ptr_err);

    if (stk->err_info.err_code & data_ptr_err)
        fprintf(stdout, "BADDATAPTR(%d) ", data_ptr_err);

    if (stk->err_info.err_code & stack_capacity_err)
        fprintf(stdout, "BADCAPACITY(%d) ", stack_capacity_err);

    if (stk->err_info.err_code & stack_size_err)
        fprintf(stdout,"BADSIZE(%d) ", stack_size_err);

    if (stk->err_info.err_code & poison_element_err)
        fprintf(stdout,"BADELEMENT(%d) ", poison_element_err);

    #ifndef NCANARY

    if (stk->err_info.err_code & end_canary_err)
        fprintf(stdout,"BADENDCANARY(%d) ", end_canary_err);

    if (stk->err_info.err_code & begin_canary_err)
        fprintf(stdout,"BADBEGINCANARY(%d) ", begin_canary_err);

    #endif
}

void StackDump(struct Stack* stk)
{

    int err_code = stk->err_info.err_code;

    fprintf(stdout,
           "\n\nStackDump()\n"
           "stack[%x] \nCREATE in %s %s:%d  \"%s\"\n",
           (unsigned int)stk,
           stk->stack_info.stack_create_func,
           stk->stack_info.stack_create_file,
           stk->stack_info.stack_create_line,
           stk->stack_info.stack_name);

    if (err_code != 0) PrintErrors(stk);

    //стурктура исправна
    if (!(err_code & stack_ptr_err)) {
        fprintf(stdout, "\n{");

        fprintf(stdout, "\n    size = %d", stk->size);
        //индекс последнего элемента не корректный
        if (err_code & stack_size_err)
            fprintf(stdout, "    (BADSIZE!)");

        fprintf(stdout, "\n    capacity = %d", stk->capacity);
        //размер массива не корректныый
        if (err_code & stack_capacity_err)
            fprintf(stdout, "    (BADCAPACITY!)");
    }

    fprintf(stdout, "\n    data[%x] ", (unsigned int)stk->data);
    //нулевой указатель на массив
    if (err_code & data_ptr_err)
        fprintf(stdout, "    (BADDATAPTR!)");

    fprintf(stdout, "\n");

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

        // Все корректно
        if (!(err_code & stack_size_err) && !(err_code & stack_capacity_err))
            PrintDataWithALLCorrect(stk);
    }

    fprintf(stdout, "}\n\n");
}

void PrintDataWithCorrectCapacity(struct Stack* stk)
{

    SET_INDEXES(stk);

    for (int i = start_index; i < end_index; i++) {
        fprintf(stdout, "    [%d] - %d", i, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNSIZE!)\n");
    }

}

void PrintDataWithCorrectSize(struct Stack* stk)
{
    SET_INDEXES(stk);

    for (int i = start_index; i < end_index; ++i) {
        fprintf(stdout, "    *[%d] - %d", i, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNCAPACITY!)\n");
    }

}

void PrintDataWithUnknownParam(struct Stack* stk)
{
    SET_INDEXES(stk);

    const int MAXVALUE = 5;
    for (int i = start_index; i < MAXVALUE; ++i) {
        fprintf(stdout, "    [%d] - %d", i, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNPARAMS!)\n");

    }

}

void PrintDataWithALLCorrect(struct Stack* stk)
{
    for (int i = start_index; i < end_index; ++i) {
        fprintf(stdout, "    ");

        if (i < stk->size)
            fprintf(stdout, "*");

        fprintf(stdout, "[%d] - %d", i, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "    (POISON!)");

        fprintf(stdout, "\n");
    }
}



