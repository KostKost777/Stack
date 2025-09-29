#ifndef STACK
#define STACK

typedef int StackValueType;

struct Stack_Info
{
    const char* stack_create_file;
    const char* stack_create_func;
    const char* stack_name;
    int stack_create_line;

    #ifdef WITHHASH
    long long hash;
    #endif
};

struct Stack
{
    StackValueType* data;
    ssize_t size;
    ssize_t capacity;
    Stack_Info stack_info;
    Error_Info err_info;
};

int StackCtor(struct Stack* stk, ssize_t stk_size);

int StackDtor(struct Stack* stk);

int StackPop(struct Stack* stk, int* last_el);

int StackPush(struct Stack* stk, int new_el);

void PrintLogs(const char* file, const char* func, int line,
               const char* err_description);

void SetStackInfo(Stack* stk, const char* stk_name,
                  const char* file, const char* func, int line);

void SetPoisonValues(Stack* stk);

int GetNewCapacity(Stack* stk);

#define INIT_STACK(stk)                                             \
    struct Stack stk = {};                                          \
    SetStackInfo(&stk, #stk, __FILE__, __func__, __LINE__);         \

#endif
