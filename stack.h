#ifndef STACK
#define STACK

typedef int Stack_t;

struct Stack
{
    Stack_t* data;
    ssize_t size;
    ssize_t capacity;
};

enum StackErr_t
{
    no_err = 0,
    stack_ptr_err = 2,
    data_ptr_err = 4,
    stack_capacity_err = 8,
    stack_size_err = 16
};

void PrintErrors(int err_code);

int StackCtor(struct Stack* stk, ssize_t stk_size);

int StackVerifier(struct Stack* stk);

void StackDump(struct Stack* stk, int err_code);

int StackDtor(struct Stack* stk);

int StackPop(struct Stack* stk, int* last_el);

int StackPush(struct Stack* stk, int new_el);

void PrintDataWithAllCorrect(struct Stack* stk);

void PrintDataWithCorrectCapacity(struct Stack* stk);

void PrintDataWithCorrectSize(struct Stack* stk);

void PrintDataWithUnknownParam(struct Stack* stk);

const ssize_t MAXCAPACITY = 1e6;
const ssize_t MAXSIZE = 1e6;

#endif
