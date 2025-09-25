#ifndef DUMP_FUNC
#define DUMP_FUNC


enum StackErr_t
{
    no_err = 0,
    stack_ptr_err = 2,
    data_ptr_err = 4,
    stack_capacity_err = 8,
    stack_size_err = 16
};

void PrintErrors(int err_code);

int StackVerifier(struct Stack* stk);

void StackDump(struct Stack* stk, int err_code);

void PrintDataWithCorrectCapacity(struct Stack* stk);

void PrintDataWithCorrectSize(struct Stack* stk);

void PrintDataWithUnknownParam(struct Stack* stk);

const ssize_t MAXCAPACITY = 1e6;
const ssize_t MAXSIZE = 1e6;

#define CHECK_STACK(stack_ptr)             \
    err_code = StackVerifier(stack_ptr);   \
    if (err_code != no_err){               \
        StackDump(stack_ptr, err_code);    \
        StackDtor(stack_ptr);              \
        return err_code;                   \
    }                                      \

#define CHECK_KOSTIK( ... )  { int err = (__VA_ARGS__); if (err) StackDtor(&stk1); return 0; }

#endif
