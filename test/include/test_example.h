/**
 * @file    test_example.h
 * @brief   Header file for `test_example.c`.
 *
 * @author  heapbadger
 * @date    August 17, 2024
 */

#ifndef TEST_EXAMPLE_H
#define TEST_EXAMPLE_H

#include <CUnit/Basic.h>

/**
 * @brief   Creates a test suite for the Hello World example.
 *
 * This function creates and returns a CUnit test suite that includes
 * the tests for the Hello World example.
 *
 * @return  Returns a pointer to the created test suite.
 */
CU_pSuite example_suite(void);

#endif // TEST_EXAMPLE_H
