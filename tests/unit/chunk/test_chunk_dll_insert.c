#include "test_chunk.h"

#include <stdlib.h>
#include <unity.h>

static void test_insert_after_middle(void);
static void test_insert_after_last(void);
static void test_insert_before_middle(void);
static void test_insert_before_head(void);

void run_chunk_dll_insert_tests(void) {
    RUN_TEST(test_insert_after_middle);
    RUN_TEST(test_insert_after_last);
    RUN_TEST(test_insert_before_middle);
    RUN_TEST(test_insert_before_head);
}

static void test_insert_after_middle(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *middle = make_chunk(32);
    chunk_header_t *tail = make_chunk(16);
    chunk_header_t *inserted = make_chunk(8);

    head->next = middle;
    middle->previous = head;
    middle->next = tail;
    tail->previous = middle;

    chunk_dll_insert_after(middle, inserted);

    TEST_ASSERT_EQUAL_PTR(middle->next, inserted);
    TEST_ASSERT_EQUAL_PTR(inserted->previous, middle);
    TEST_ASSERT_EQUAL_PTR(inserted->next, tail);
    TEST_ASSERT_EQUAL_PTR(tail->previous, inserted);

    free(head);
    free(middle);
    free(tail);
    free(inserted);
}

static void test_insert_after_last(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *tail = make_chunk(32);
    chunk_header_t *inserted = make_chunk(16);

    head->next = tail;
    tail->previous = head;

    chunk_dll_insert_after(tail, inserted);

    TEST_ASSERT_EQUAL_PTR(tail->next, inserted);
    TEST_ASSERT_EQUAL_PTR(inserted->previous, tail);
    TEST_ASSERT_NULL(inserted->next);

    free(head);
    free(tail);
    free(inserted);
}

static void test_insert_before_middle(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *middle = make_chunk(32);
    chunk_header_t *tail = make_chunk(16);
    chunk_header_t *inserted = make_chunk(8);

    head->next = middle;
    middle->previous = head;
    middle->next = tail;
    tail->previous = middle;

    chunk_dll_insert_before(middle, inserted);

    TEST_ASSERT_EQUAL_PTR(head->next, inserted);
    TEST_ASSERT_EQUAL_PTR(inserted->previous, head);
    TEST_ASSERT_EQUAL_PTR(inserted->next, middle);
    TEST_ASSERT_EQUAL_PTR(middle->previous, inserted);

    free(head);
    free(middle);
    free(tail);
    free(inserted);
}

static void test_insert_before_head(void) {
    chunk_header_t *head = make_chunk(64);
    chunk_header_t *inserted = make_chunk(32);

    chunk_dll_insert_before(head, inserted);

    TEST_ASSERT_EQUAL_PTR(inserted->next, head);
    TEST_ASSERT_NULL(inserted->previous);
    TEST_ASSERT_EQUAL_PTR(head->previous, inserted);

    free(head);
    free(inserted);
}
