#include <TXLib.h>

#include "dump_functions.h"
#include "stack_functions.h"

const char* log_file_name = "file_with_errors.txt";

void SetErrorInfo(Stack* stk,
                  const char* file, const char* func, int line)
{
    stk->err_info.err_file = file;
    stk->err_info.err_func = func;
    stk->err_info.err_line = line;
}

#ifdef WITHHASH

long long int GetHash(Stack* stk)
{
    long long int hash = 4113;

    for (int i = CANARY_CONST; i < stk->capacity + CANARY_CONST; ++i)
        hash = ((hash << 7) + hash) + (int)stk->data[i];

    return hash;
}

int CheckHash(Stack* stk, long long int hash)
{
    if (hash != stk->stack_info.hash)
        return hash_alarm;
    else
        return no_err;
}

#endif

int StackVerifier(struct Stack* stk)
{
    if (stk == NULL)
        stk->err_info.err_code |= stack_ptr_err;

    if (stk->data == NULL)
        stk->err_info.err_code |= data_ptr_err;

    if (stk->capacity <= 0 || stk->capacity >= MAXCAPACITY)
        stk->err_info.err_code |= stack_capacity_err;

    #ifdef WITHCANARY

    if (stk->data != NULL && stk->capacity > 0 && stk->capacity < MAXCAPACITY) {

        if (stk->data[0] != CANARY)
            stk->err_info.err_code |= begin_canary_err;

        if (stk->data[stk->capacity + 1] != CANARY)
            stk->err_info.err_code |= end_canary_err;
    }

    #endif

    #ifdef WITHHASH

    if (stk->data != NULL && stk->capacity > 0 && stk->capacity < MAXCAPACITY) {
        if (CheckHash(stk, GetHash(stk)))
            stk->err_info.err_code |= hash_alarm;
    }

    #endif

    if (stk->size < CANARY_CONST || stk->size >= MAXSIZE)
        stk->err_info.err_code |= stack_size_err;

    else {
        if (stk->size > CANARY_CONST + 1)
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

    #ifdef WITHCANARY

    if (stk->err_info.err_code & end_canary_err)
        fprintf(stdout,"BADENDCANARY(%d) ", end_canary_err);

    if (stk->err_info.err_code & begin_canary_err)
        fprintf(stdout,"BADBEGINCANARY(%d) ", begin_canary_err);

    #endif

    #ifdef WITHHASH

    if (stk->err_info.err_code & hash_alarm)
        fprintf(stdout,"HASHALARM(%d) ", hash_alarm);

    #endif
}

void StackDump(struct Stack* stk)
{

    int err_code = stk->err_info.err_code;

    fprintf(stdout,
           "\n\nStackDump()\n"
           "stack[0x%p] \nCREATE in %s %s:%d  \"%s\"\n",
           stk,
           stk->stack_info.stack_create_func,
           stk->stack_info.stack_create_file,
           stk->stack_info.stack_create_line,
           stk->stack_info.stack_name);

    if (err_code != 0) PrintErrors(stk);

    //стурктура исправна
    if (!(err_code & stack_ptr_err)) {
        fprintf(stdout, "\n{");

        fprintf(stdout, "\n    size = %d", stk->size - CANARY_CONST);
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
            PrintDataWithUnknownParam(stk, 10);

        // Все корректно
        if (!(err_code & stack_size_err) && !(err_code & stack_capacity_err))
            PrintDataWithALLCorrect(stk);
    }

    fprintf(stdout, "}\n\n");
}

void PrintDataWithCorrectCapacity(struct Stack* stk)
{
    for (int i = CANARY_CONST; i < stk->capacity + CANARY_CONST; i++) {
        fprintf(stdout, "    [%d] - %d", i - CANARY_CONST, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNSIZE!)\n");
    }

}

void PrintDataWithCorrectSize(struct Stack* stk)
{
    for (int i = CANARY_CONST; i < stk->capacity + CANARY_CONST; ++i) {
        fprintf(stdout, "    *[%d] - %d", i - CANARY_CONST, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNCAPACITY!)\n");
    }

}

void PrintDataWithUnknownParam(struct Stack* stk, int max_lines)
{
    for (int i = CANARY_CONST; i < max_lines; ++i) {
        fprintf(stdout, "    [%d] - %d", i - CANARY_CONST, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "   (POISON)\n");
        else
            fprintf(stdout, "   (UNKNOWNPARAMS!)\n");

    }

}

void PrintDataWithALLCorrect(struct Stack* stk)
{
    for (int i = CANARY_CONST; i < stk->capacity + CANARY_CONST; ++i) {
        fprintf(stdout, "    ");

        if (i < stk->size)
            fprintf(stdout, "*");

        fprintf(stdout, "[%d] - %d", i - CANARY_CONST, stk->data[i]);

        if (stk->data[i] == POISON)
            fprintf(stdout, "    (POISON!)");

        fprintf(stdout, "\n");
    }
}



