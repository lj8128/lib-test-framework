#ifndef TEST_EXECUTION_H
#define TEST_EXECUTION_H

#include "test_setup_and_cleanup.h"

int run_specific_test(TestCatalog* catalog, const char* test_name);
int run_all_tests(TestCatalog* catalog);
void print_freq(const char* msg);

#endif