#include "test_chunk.h"

#include <stdlib.h>
#include <unity.h>

static void test_transfer_chunk_to_empty_list(void);
static void test_transfer_chunk_to_non_empty_list(void);
static void test_transfer_chunk_with_middle_insertion(void);

void run_chunk_dll_transfer_sorted_tests(void) {
    RUN_TEST(test_transfer_chunk_to_empty_list);
    RUN_TEST(test_transfer_chunk_to_non_empty_list);
    RUN_TEST(test_transfer_chunk_with_middle_insertion);
}

static void test_transfer_chunk_to_empty_list(void) {
    chunk_header_t *src = make_chunk(64);
    chunk_header_t *dest_head = NULL;

    chunk_dll_transfer_sorted(&dest_head, src);

    TEST_ASSERT_EQUAL_PTR(src, dest_head);
    TEST_ASSERT_NULL(src->next);
    TEST_ASSERT_NULL(src->previous);

    free(src);
}

static void test_transfer_chunk_to_non_empty_list(void) {
    chunk_header_t *dest_head = make_chunk(32);
    chunk_header_t *src = make_chunk(64);

    if (dest_head > src) {
        chunk_header_t *tmp = dest_head;
        dest_head = src;
        src = tmp;
    }
    chunk_dll_transfer_sorted(&dest_head, src);

    TEST_ASSERT_EQUAL_PTR(src, dest_head->next);
    TEST_ASSERT_EQUAL_PTR(dest_head, src->previous);
    TEST_ASSERT_NULL(src->next);
    TEST_ASSERT_NULL(dest_head->previous);

    free(src);
    free(dest_head);
}

static void test_transfer_chunk_with_middle_insertion(void) {
    chunk_header_t *first = make_chunk(32);
    chunk_header_t *middle = make_chunk(64);
    chunk_header_t *last = make_chunk(128);

    if (first > middle) {
        chunk_header_t *tmp = first;
        first = middle;
        middle = tmp;
    }
    if (middle > last) {
        chunk_header_t *tmp = middle;
        middle = last;
        last = tmp;
    }
    first->next = middle;
    middle->previous = first;
    middle->next = last;
    last->previous = middle;

    chunk_header_t *dest_head = first;
    chunk_dll_transfer_sorted(&dest_head, middle);

    // The list should now have first -> middle -> last, and middle should be in
    // the right position
    TEST_ASSERT_EQUAL_PTR(first, dest_head);
    TEST_ASSERT_NULL(first->previous);
    TEST_ASSERT_EQUAL_PTR(middle, first->next);
    TEST_ASSERT_EQUAL_PTR(first, middle->previous);
    TEST_ASSERT_EQUAL_PTR(last, middle->next);
    TEST_ASSERT_EQUAL_PTR(middle, last->previous);
    TEST_ASSERT_NULL(last->next);

    free(first);
    free(middle);
    free(last);
}
