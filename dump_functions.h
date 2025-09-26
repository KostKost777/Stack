#ifndef DUMP_FUNC
#define DUMP_FUNC

const int POISON = 228;

const int CANARY = INT_MIN;

enum StackErr_t
{
    no_err = 0,
    stack_ptr_err = 2,
    data_ptr_err = 4,
    stack_capacity_err = 8,
    stack_size_err = 16,
    poison_element_err = 32,
    stack_canary_err = 64
};

struct Error_Info
{
    int err_code;
    const char* err_file;
    const char* err_func;
    int err_line;
};

void PrintErrors(struct Stack* stk);

int StackVerifier(struct Stack* stk);

void StackDump(struct Stack* stk);

void PrintDataWithCorrectCapacity(struct Stack* stk);

void PrintDataWithCorrectSize(struct Stack* stk);

void PrintDataWithUnknownParam(struct Stack* stk);

void PrintDataWithALLCorrect(struct Stack* stk);

const ssize_t MAXCAPACITY = 1e6;
const ssize_t MAXSIZE = 1e6;

#define CHECK_STACK(stk)             \
    if (StackVerifier(stk) != no_err){              \
        stk->err_info.err_file = __FILE__;               \
        stk->err_info.err_func = __func__;               \
        stk->err_info.err_line = __LINE__;               \
        StackDump(stk);                                     \
        return stk->err_info.err_code;                   \
    }                                                       \

#define CHECK_ERROR(FUNC, stk)  \
    if (FUNC) { \
    StackDtor(&stk);   \
    return 0;     \
    } \

#endif
