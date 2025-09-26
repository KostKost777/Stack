#ifndef STACK
#define STACK

typedef int Stack_t;

struct Stack_Info
{
    const char* stack_create_file;
    const char* stack_create_func;
    char* stack_name;
    int stack_create_line;
};

struct Stack
{
    Stack_t* data;
    ssize_t size;
    ssize_t capacity;
    Stack_Info stack_info;
    Error_Info err_info;
};

int StackCtor(struct Stack* stk, ssize_t stk_size);

int StackDtor(struct Stack* stk);

int StackPop(struct Stack* stk, int* last_el);

int StackPush(struct Stack* stk, int new_el);

#define INIT_STACK(stk) \
    struct Stack stk = {};   \
    stk.stack_info.stack_create_file = __FILE__; \
    stk.stack_info.stack_create_func = __func__; \
    stk.stack_info.stack_create_line = __LINE__; \
    stk.stack_info.stack_name = #stk; \


#define GETNAME(name) #name

#endif
