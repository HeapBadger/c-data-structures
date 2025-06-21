/**
 * @file    test_main.c
 *
 * @author  heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "test_array.h"
#include "test_auxiliary.h"
#include "test_linked_list.h"
#include "test_matrix.h"
#include "test_stack.h"
#include "test_queue.h"

static void print_help(void);
static int  create_suites(void);
static void list_suites(void);
static void run_suite(const char *suite_name);

/**
 * @brief   Main function to run all tests.
 *
 * @param argc  Number of command-line arguments.
 * @param argv  Array of command-line arguments.
 *
 * @return  Returns CUE_SUCCESS on success, or a non-zero value on failure.
 */
int
main (int argc, char *argv[])
{
    int retval = CUE_SUCCESS;
    CU_basic_set_mode(CU_BRM_VERBOSE);

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        ERROR_LOG("Failed to initialize CUnit registry\n");
        retval = CU_get_error();
        goto CLEANUP;
    }

    retval = create_suites();

    if (CUE_SUCCESS != retval)
    {
        ERROR_LOG("Failed to create CUnit test suites\n");
        goto CLEANUP;
    }

    if (1 == argc)
    {
        CU_basic_run_tests();
    }
    else if (2 == argc)
    {
        if (is_name_match("help", argv[1]))
        {
            print_help();
        }
        else if (is_name_match("list", argv[1]))
        {
            list_suites();
        }
        else
        {
            run_suite(argv[1]);
        }
    }
    else
    {
        print_help();
    }

CLEANUP:
    CU_cleanup_registry();
    return retval;
}

/**
 * @brief   Print help instructions.
 */
static void
print_help (void)
{
    printf("Usage: test_main [command]\n");
    printf("Commands:\n");
    printf("  help          Print this help message\n");
    printf("  list          Lists all available test suites\n");
    printf("  <suite_name>  Run the specified test suite\n");
    printf("  (no argument) Run all test suites\n");
    return;
}

/**
 * @brief   List all available test suites.
 */
static void
list_suites (void)
{
    CU_pSuite pSuite = NULL;
    printf("Available Suites:\n");

    for (pSuite = CU_get_registry()->pSuite; pSuite != NULL;
         pSuite = pSuite->pNext)
    {
        printf("\t%s\n", pSuite->pName);
    }

    printf("\n");
}

/**
 * @brief   Run the specified test suite.
 *
 * @param suite_name  Name of the suite to run.
 */
static void
run_suite (const char *suite_name)
{
    CU_pSuite pSuite        = NULL;
    CU_pSuite current_suite = NULL;

    for (pSuite = CU_get_registry()->pSuite; pSuite != NULL;
         pSuite = pSuite->pNext)
    {
        if (is_name_match(suite_name, pSuite->pName))
        {
            current_suite = pSuite;
            break;
        }
    }

    if (NULL == current_suite)
    {
        ERROR_LOG("'%s' not found", suite_name);
    }
    else
    {
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_suite(current_suite);
    }

    return;
}

/**
 * @brief   Create all test suites.
 *
 * @return  Returns CUE_SUCCESS on success, or a non-zero value on failure.
 */
static int
create_suites (void)
{
    int retval = CUE_SUCCESS;

    // Linked List
    if (NULL == ll_suite())
    {
        ERROR_LOG("Failed to create the Linked List Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

    // Array
    if (NULL == array_suite())
    {
        ERROR_LOG("Failed to create the Array Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

    // Matrix
    if (NULL == matrix_suite())
    {
        ERROR_LOG("Failed to create the Matrix Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

    // Stack
    if (NULL == stack_suite())
    {
        ERROR_LOG("Failed to create the Stack Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

    // Queue
    if (NULL == queue_suite())
    {
        ERROR_LOG("Failed to create the Queue Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

EXIT:
    return retval;
}

/*** end of file ***/