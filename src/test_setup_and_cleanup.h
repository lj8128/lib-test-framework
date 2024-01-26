#ifndef TEST_SETUP_AND_CLEANUP_H
#define TEST_SETUP_AND_CLEANUP_H

#include <stdbool.h>
#include "status_test_framework.h"
#include "hash_table.h"

typedef struct test {
    const char* test_name;
    TestResult (*test_function)();
    RoutineResult (*pre_test_routine)();
    RoutineResult (*post_test_routine)();
    struct test* next;
} Test;

typedef struct {
    HashTable* test_registry;

    Test* head_test;
    Test* tail_test;
} TestCatalog;

int init_test_catalog(TestCatalog* catalog);
int register_test(TestCatalog* catalog, const char* test_name, TestResult (*test_function)());
int register_before_each_routine(TestCatalog* catalog, RoutineResult (*before_each_routine)());
int register_after_each_routine(TestCatalog* catalog, RoutineResult (*after_each_routine)());
int delete_test_catalog(TestCatalog** catalog_ref);

#endif