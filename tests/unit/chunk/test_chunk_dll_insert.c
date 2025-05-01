#include "test_chunk.h"

#include <unity.h>

static void test_insert_into_empty_list(void);
static void test_insert_after_head(void);
static void test_insert_in_middle(void);

void run_chunk_dll_insert_tests(void) {
    RUN_TEST(test_insert_into_empty_list);
    RUN_TEST(test_insert_after_head);
    RUN_TEST(test_insert_in_middle);
}

static void test_insert_into_empty_list(void) {
    chunk_header_t *head = NULL;
    chunk_header_t *chunk = make_chunk(64, 0);

    chunk_dll_insert(&head, chunk);

    TEST_ASSERT_EQUAL_PTR(chunk, head);
    TEST_ASSERT_NULL(chunk->next);
    TEST_ASSERT_NULL(chunk->previous);

    free(chunk);
}

static void test_insert_after_head(void) {
    chunk_header_t *head = make_chunk(64, 0);
    chunk_header_t *second = make_chunk(32, 0);

    chunk_dll_insert(&head, second);

    TEST_ASSERT_EQUAL_PTR(second, head->next);
    TEST_ASSERT_EQUAL_PTR(head, second->previous);
    TEST_ASSERT_NULL(second->next);

    free(second);
    free(head);
}

static void test_insert_in_middle(void) {
    // Create the chunks
    chunk_header_t *head = make_chunk(64, 0);
    chunk_header_t *middle = make_chunk(32, 0);
    chunk_header_t *tail = make_chunk(16, 0);

    // Link the chunks: head <-> tail
    head->next = tail;
    tail->previous = head;

    // Insert the middle chunk between head and tail
    chunk_dll_insert(&head, middle); // Insert after head

    // Verify the list
    TEST_ASSERT_EQUAL_PTR(middle, head->next);
    TEST_ASSERT_EQUAL_PTR(tail, middle->next);
    TEST_ASSERT_EQUAL_PTR(head, middle->previous);
    TEST_ASSERT_EQUAL_PTR(middle, tail->previous);

    // Check that the previous and next pointers of head and tail are correctly
    // set
    TEST_ASSERT_NULL(head->previous);
    TEST_ASSERT_NULL(tail->next);

    free(head);
    free(middle);
    free(tail);
}
