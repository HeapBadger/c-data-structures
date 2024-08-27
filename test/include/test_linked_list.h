/**
 * @file    test_linked_list.h
 * @brief   Header file for `test_linked_list.c`.
 *
 * @author  Anna DeVries
 * @date    August 21, 2024
 */

#ifndef TEST_LINKED_LIST_H
#define TEST_LINKED_LIST_H

#include <CUnit/Basic.h>

/**
 * @brief   Creates a test suite for the Linked List file.
 *
 * @return  Returns a pointer to the created test suite.
 */
CU_pSuite linked_list_suite(void);

#endif // TEST_LINKED_LIST_H
