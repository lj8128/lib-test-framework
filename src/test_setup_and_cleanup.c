#include "test_setup_and_cleanup.h"
#include "config_test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_test_catalog(TestCatalog* catalog) {
    int res = SUCCESS;

    catalog->head_test = 0;
    catalog->tail_test = 0;
    catalog->test_registry = malloc(sizeof(HashTable));

    if(catalog->test_registry == 0) {
        res = EMEM;
        goto out;
    }

    init_table(catalog->test_registry, MAX_TEST_REGISTRY_SLOTS);

out:
    if(res == EMEM) printf("EMEM: Failed to allocate memory for "
                           "test_registry.\n");
    return res;
}

static void insert_test_node(TestCatalog* catalog, Test* new_test) {
    if(catalog->head_test == 0) {
        catalog->head_test = new_test;
        catalog->tail_test = new_test;
    } else {
        catalog->tail_test->next = new_test;
        catalog->tail_test = new_test;
    }
}

int register_test(TestCatalog* catalog, const char* test_name, TestResult (*test_function)()) {
    int res = SUCCESS;

    if(search_for_entry(catalog->test_registry, test_name) != 0) {
        res = EINVARG;
        goto out;
    }

    Test* new_test = calloc(1, sizeof(Test));
    if(new_test == 0) {
        res = EMEM;
        goto out;
    }
    new_test->test_name = strdup(test_name);
    new_test->test_function = test_function;

    insert_entry(catalog->test_registry, test_name, (void*) new_test);
    insert_test_node(catalog, new_test);

out:
    if(res == EINVARG) printf("EINVARG: Test name '%s' already exists!\n", 
                              test_name);
    if(res == EMEM) printf("EMEM: Failed to allocate memory for new Test.\n");
    return res;
}

int register_before_each_routine(TestCatalog* catalog, RoutineResult (*before_each_routine)()) {
    int res = SUCCESS;
    if(catalog == 0 || before_each_routine == 0) {
        res = EINVARG;
        goto out;
    }

    Test* cur_test = catalog->head_test;
    while(cur_test != 0) {
        cur_test->pre_test_routine = before_each_routine;
        cur_test = cur_test->next;
    }

out:
    if(res == EINVARG) printf("`catalog` and `before_each_routine` must not be "
                              "null!\n");
    return res;
}

int register_after_each_routine(TestCatalog* catalog, RoutineResult (*after_each_routine)()) {
    int res = SUCCESS;
    if(catalog == 0 || after_each_routine == 0) {
        res = EINVARG;
        goto out;
    }

    Test* cur_test = catalog->head_test;
    while(cur_test != 0) {
        cur_test->post_test_routine = after_each_routine;
        cur_test = cur_test->next;
    }

out:
    if(res == EINVARG) printf("`catalog` and `after_each_routine` must not be "
                              "null!\n");
    return res;
}

static void delete_tests(TestCatalog* catalog) {
    Test* cur_test = catalog->head_test;

    while(cur_test != 0) {
        Test* tmp = cur_test->next;
        free((void*) cur_test->test_name);
        free(cur_test);
        cur_test = tmp;
    }

    catalog->head_test = 0;
    catalog->tail_test = 0;
}

int delete_test_catalog(TestCatalog** catalog_ref) {
    int res = SUCCESS;

    if(catalog_ref == 0 || *catalog_ref == 0) {
        res = EINVARG;
        goto out;     
    }

    TestCatalog* catalog = *catalog_ref;

    delete_tests(catalog);
    delete_table(&catalog->test_registry);
    free(catalog);
    *catalog_ref = 0;

out:
    if(res == EINVARG) printf("EINVARG: `catalog_ref` and `*catalog_ref` must not be 0!\n");
    return res;
}