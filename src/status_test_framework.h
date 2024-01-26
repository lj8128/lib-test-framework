#ifndef STATUS_TEST_FRAMEWORK_H 
#define STATUS_TEST_FRAMEWORK_H

#define SUCCESS 0
#define EMEM -1
#define EINVARG -2
#define EPREROUT -3
#define EPOSTROUT -4
#define ENOTEST -5

typedef enum {
    TEST_PASSED = 0,
    TEST_FAILED 
} TestResult;

typedef enum {
    ROUTINE_SUCCEEDED = 0,
    ROUTINE_FAILED 
} RoutineResult;

#endif