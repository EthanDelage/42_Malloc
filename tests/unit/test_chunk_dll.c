#include "chunk_dll.h"

#include <stdlib.h>
#include <unity.h>

static void test_insert_into_empty_list(void);
static void test_insert_after_head(void);
static void test_insert_in_middle(void);
static void test_remove_middle_node(void);
static void test_remove_head_node(void);
static void test_remove_last_chunk(void);
static chunk_header_t *make_chunk(size_t size, int in_use);

void run_chunk_dll_tests(void) {
    RUN_TEST(test_insert_into_empty_list);
    RUN_TEST(test_insert_after_head);
    RUN_TEST(test_insert_in_middle);
    RUN_TEST(test_remove_middle_node);
    RUN_TEST(test_remove_head_node);
    RUN_TEST(test_remove_last_chunk);
}

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
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

static void test_remove_middle_node(void) {
    chunk_header_t *head = make_chunk(64, 0);
    chunk_header_t *middle = make_chunk(32, 0);
    chunk_header_t *tail = make_chunk(16, 0);

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
    chunk_header_t *head = make_chunk(64, 0);
    chunk_header_t *second = make_chunk(32, 0);

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
    chunk_header_t *head = make_chunk(64, 0);
    chunk_header_t *tail = make_chunk(32, 0);

    head->next = tail;
    tail->previous = head;

    chunk_dll_remove(&tail);

    TEST_ASSERT_NULL(head->next);
    TEST_ASSERT_NULL(tail->next);
    TEST_ASSERT_NULL(tail->previous);

    free(head);
    free(tail);
}

static chunk_header_t *make_chunk(size_t size, int in_use) {
    chunk_header_t *chunk = malloc(sizeof(chunk_header_t));
    chunk->size = size;
    chunk->in_use = in_use;
    chunk->next = NULL;
    chunk->previous = NULL;
    return chunk;
}
