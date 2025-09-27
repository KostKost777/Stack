#ifndef DUMP_FUNC
#define DUMP_FUNC

const int POISON = 228;

#ifndef NCANARY

int CANARY = INT_MIN;
const int CANARY_CONST = 1;

#else

const int CANARY_CONST = 0;

#endif

enum StackErr_t
{
    no_err = 0,
    stack_ptr_err = 2,
    data_ptr_err = 4,
    stack_capacity_err = 8,
    stack_size_err = 16,
    poison_element_err = 32,
    end_canary_err = 64,
    begin_canary_err = 128
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

void SetErrorInfo(Stack* stk,
                  const char* file, const char* func, int line);

const ssize_t MAXCAPACITY = 1e6;
const ssize_t MAXSIZE = 1e6;

#define CHECK_STACK(stk_ptr)             \
    if (StackVerifier(stk_ptr) != no_err){\
        SetErrorInfo(stk_ptr, __FILE__, __func__, __LINE__);    \
        StackDump(stk_ptr);                                     \
        return stk_ptr->err_info.err_code;                   \
    }                                                       \

#define PRINT_LOGS(message)  \
    PrintLogs(__FILE__, __func__, __LINE__, message);

#endif


