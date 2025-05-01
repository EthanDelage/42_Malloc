#include "test_chunk.h"

#include <unity.h>

static void test_remove_middle_node(void);
static void test_remove_head_node(void);
static void test_remove_last_chunk(void);

void run_chunk_dll_remove_tests(void) {
    RUN_TEST(test_remove_middle_node);
    RUN_TEST(test_remove_head_node);
    RUN_TEST(test_remove_last_chunk);
}

static void test_remove_middle_node(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *middle = make_chunk(32);
    chunk_header_t *tail = make_chunk(16);

    head->next = middle;
    middle->previous = head;
    middle->next = tail;
    tail->previous = middle;

    chunk_dll_remove(&middle);

    TEST_ASSERT_EQUAL_PTR(head->next, tail);
    TEST_ASSERT_EQUAL_PTR(tail->previous, head);
    TEST_ASSERT_NULL(middle->next);
    TEST_ASSERT_NULL(middle->previous);

    free(head);
    free(middle);
    free(tail);
}

static void test_remove_head_node(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *second = make_chunk(32);

    head->next = second;
    second->previous = head;

    chunk_header_t *removal_target = head;
    chunk_dll_remove(&removal_target);

    TEST_ASSERT_EQUAL_PTR(second, removal_target);
    TEST_ASSERT_NULL(second->previous);
    TEST_ASSERT_NULL(head->next);
    TEST_ASSERT_NULL(head->previous);

    free(second);
    free(head);
}

static void test_remove_last_chunk(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *tail = make_chunk(32);

    head->next = tail;
    tail->previous = head;

    chunk_dll_remove(&tail);

    TEST_ASSERT_NULL(head->next);
    TEST_ASSERT_NULL(tail->next);
    TEST_ASSERT_NULL(tail->previous);

    free(head);
    free(tail);
}
