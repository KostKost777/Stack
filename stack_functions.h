#ifndef STACK
#define STACK

typedef int Stack_t;

struct Stack
{
    Stack_t* data;
    ssize_t size;
    ssize_t capacity;
};

int StackCtor(struct Stack* stk, ssize_t stk_size);

int StackDtor(struct Stack* stk);

int StackPop(struct Stack* stk, int* last_el);

int StackPush(struct Stack* stk, int new_el);

#endif
