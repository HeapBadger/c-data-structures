/**
 * @file    test_example.c
 * @brief   Example test suite.
 *
 * This file contains the test suite creation for 'example.c'.
 *
 * @author  Anna DeVries
 * @date    August 17, 2024
 */

#include "test_example.h"
#include "example.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function Declarations
static void test_hello(void);

// Suite Creation Funtion
CU_pSuite example_suite(void) {
  CU_pSuite suite = NULL;
  suite = CU_add_suite("example-suite", 0, 0);

  if (NULL == suite) {
    ERROR_LOG("Failed to add example-suite\n");
    goto CLEANUP;
  }

  if (NULL == (CU_add_test(suite, "test_hello", test_hello))) {
    ERROR_LOG("Failed to add test_hello to suite\n");
    suite = NULL;
    goto CLEANUP;
  }

CLEANUP:
  if (NULL == suite) {
    CU_cleanup_registry();
  }

  return suite;
}

/**
 * @brief   Test case for the hello function.
 *
 * This test verifies that the hello function prints "Hello World" and returns
 * 0. It captures the output and checks against the expected output.
 *
 * @return  None.
 */
static void test_hello(void) {
  // Capture function output to buffer
  char *buffer = NULL;
  buffer = capture_stdout(hello, NULL);

  // Check the output and return value
  CU_ASSERT_STRING_EQUAL(buffer, "Hello World!\n");

  // Clean up
  free(buffer);
  buffer = NULL;
  return;
}
