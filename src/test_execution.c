#include "test_execution.h"
#include "hash_table.h"
#include "config_test_framework.h"
#include "status_test_framework.h"
#include "status_tests_and_routines.h"
#include <stdio.h>

static int run_test(Test* test, int* num_failed_tests, int* test_count) {
    int res = SUCCESS;

    if(test->pre_test_routine != 0 && test->pre_test_routine()) {
        res = EPREROUT;
        goto out;
    }        

    if(test_count == 0) {
        printf("-->RUNNING TEST: %s\n", test->test_name);
    } else {
        printf("-->RUNNING TEST %d: %s\n", (*test_count)++, test->test_name);
    }
    TestResult test_result = test->test_function();
    if(test_result == TEST_FAILED) {
        printf(FAILED_COLOR "-->FAILED: %s----------------------"
               "-------------\n" COLOR_RESET, test->test_name);
        if(num_failed_tests != 0) ++(*num_failed_tests);
    } else {
        printf(PASSED_COLOR "-->PASSED: %s----------------------"
               "-------------\n" COLOR_RESET, test->test_name);
    }

    if(test->post_test_routine != 0 && test->post_test_routine()) {
        res = EPOSTROUT;
        goto out;
    }

out:
    return res;
}

static int iterate_through_tests(TestCatalog* catalog, int* num_failed_tests) {
    int res = SUCCESS; 

    Test* cur_test = catalog->head_test;

    int test_count = 1;

    while(cur_test != 0) {
        res = run_test(cur_test, num_failed_tests, &test_count); 
        if(res == EPREROUT || res == EPOSTROUT) goto out; 
        cur_test = cur_test->next;
    }

out:
    return res;
}

int run_specific_test(TestCatalog* catalog, const char* test_name) {
    int res = SUCCESS;

    if(catalog == 0 || test_name == 0) {
        res = EINVARG;
        goto out;
    }

    TableEntry* test_entry = search_for_entry(catalog->test_registry, test_name);
    if(test_entry == 0) {
        res = ENOTEST;
        goto out; 
    }

    Test* test = (Test*)test_entry->value;
    res = run_test(test, 0, 0);
    if(res == EPREROUT || res == EPOSTROUT) goto out; 

out:
    if(res == EINVARG) printf("EINVARG:" TERM_TEST "`catalog` and `test_name`"
                              " must not be null!\n");
    if(res == ENOTEST) printf("EINVARG:" TERM_TEST "%s is not a registered "
                              "test!\n", test_name);
    if(res == EPREROUT) printf("EPREROUT:" TERM_TEST "`pre_test_routine` "
                               "failed!\n");
    if(res == EPOSTROUT) printf("EPOSTROUT:" TERM_TEST "`post_test_routine` "
                                "failed!\n");
    return res;
}

int run_all_tests(TestCatalog* catalog) {
    int res = SUCCESS;

    if(catalog == 0) {
        res = EINVARG;
        goto out;
    }

    int num_failed_tests = 0;

    res = iterate_through_tests(catalog, &num_failed_tests);
    if(res == EPREROUT || res == EPOSTROUT) goto out;

    if(num_failed_tests == 0) {
        printf(PASSED_COLOR "===RESULT: All tests passed!======================"
               "=============\n" COLOR_RESET);
    } else {
        int num_total_tests = catalog->test_registry->size;
        printf(FAILED_COLOR "===RESULT: %d / %d tests failed!==="
               "================================\n" COLOR_RESET, num_failed_tests, 
               num_total_tests);
    }

out:
    if(res == EINVARG) printf("EINVARG:" TERM_TEST "`catalog` must not be null!\n");
    if(res == EPREROUT) printf("EPREROUT:" TERM_TEST "`pre_test_routine` failed!\n");
    if(res == EPOSTROUT) printf("EPOSTROUT:" TERM_TEST "`post_test_routine` failed!\n");
    return res;
}

void print_freq(const char* msg) {
    printf(FAILED_COLOR "Failed Requirement: %s\n" COLOR_RESET, msg);
}