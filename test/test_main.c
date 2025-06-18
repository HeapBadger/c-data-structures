/**
 * @file    test_main.c
 * @brief   Control suite for all test cases.
 *
 * @author  heapbadger
 * @date    August 17, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "test_array.h"
#include "test_auxiliary.h"
#include "test_doubly_linked_list.h"
#include "test_linked_list.h"
#include "test_matrix.h"
#include "test_stack.h"

// Function Declarations
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
 * @return  Returns 0 on success, or a non-zero value on failure.
 */
int
main (int argc, char *argv[])
{
    int retval = CUE_SUCCESS;
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // Initialize the CUnit registry
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        ERROR_LOG("Failed to initialize CUnit registry\n");
        retval = CU_get_error();
        goto CLEANUP;
    }

    // Create all test suites
    retval = create_suites();

    if (CUE_SUCCESS != retval)
    {
        ERROR_LOG("Failed to create CUnit test suites\n");
        goto CLEANUP;
    }

    // Process command-line arguments
    if (1 == argc)
    {
        CU_basic_run_tests();
    }
    else if (2 == argc)
    {
        // Process the command
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
 *
 * @return  Returns 0 on success, or a non-zero value on failure.
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
 * This should be the only function that needs to be updated. This is
 * where a developer may add additional suites to be created. See the
 * "example_suite" as an example.
 *
 * @return  Returns 0 on success, or a non-zero value on failure.
 */
static int
create_suites (void)
{
    int retval = CUE_SUCCESS; // Assume success unless an error occurs

    // Linked List
    if (NULL == ll_suite())
    {
        ERROR_LOG("Failed to create the Linked List Suite\n");
        retval = CU_get_error();
        goto EXIT;
    }

    // Doubly Linked List
    if (NULL == dl_suite())
    {
        ERROR_LOG("Failed to create the Doubly Linked List Suite\n");
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

EXIT:
    return retval;
}

/*** end of file ***/