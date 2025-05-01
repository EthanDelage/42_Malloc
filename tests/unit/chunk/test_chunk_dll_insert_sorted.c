#include "test_chunk.h"

#include <unity.h>

static void test_insert_sorted_empty_list(void);
static void test_insert_sorted_at_head(void);
static void test_insert_sorted_in_middle(void);
static void test_insert_sorted_at_tail(void);

void run_chunk_dll_insert_sorted_tests(void) {
    RUN_TEST(test_insert_sorted_empty_list);
    RUN_TEST(test_insert_sorted_at_head);
    RUN_TEST(test_insert_sorted_in_middle);
    RUN_TEST(test_insert_sorted_at_tail);
}

static void test_insert_sorted_empty_list(void) {
    chunk_header_t *head = NULL;
    chunk_header_t *new_chunk = make_chunk(64);

    chunk_dll_insert_sorted(&head, new_chunk);

    TEST_ASSERT_EQUAL_PTR(head, new_chunk);
    TEST_ASSERT_NULL(new_chunk->previous);
    TEST_ASSERT_NULL(new_chunk->next);

    free(new_chunk);
}

static void test_insert_sorted_at_head(void) {
    chunk_header_t *chunk1 = make_chunk(64);
    chunk_header_t *chunk2 = make_chunk(64); // Address before chunk1

    // Ensure chunk2 has a lower address
    if (chunk2 > chunk1) {
        chunk_header_t *tmp = chunk1;
        chunk1 = chunk2;
        chunk2 = tmp;
    }
    chunk_header_t *head = chunk1;

    chunk_dll_insert_sorted(&head, chunk2);

    TEST_ASSERT_EQUAL_PTR(head, chunk2);
    TEST_ASSERT_EQUAL_PTR(chunk2->next, chunk1);
    TEST_ASSERT_NULL(chunk2->previous);
    TEST_ASSERT_EQUAL_PTR(chunk1->previous, chunk2);
    TEST_ASSERT_NULL(chunk1->next);
    TEST_ASSERT_NULL(chunk2->previous);

    free(chunk1);
    free(chunk2);
}
static void test_insert_sorted_in_middle(void) {
    chunk_header_t *chunk1 = make_chunk(64);
    chunk_header_t *chunk2 = make_chunk(64);
    chunk_header_t *chunk3 = make_chunk(64);

    // Sort manually
    chunk_header_t *chunks[] = { chunk1, chunk2, chunk3 };
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (chunks[j] < chunks[i]) {
                chunk_header_t *tmp = chunks[i];
                chunks[i] = chunks[j];
                chunks[j] = tmp;
            }
        }
    }

    chunk_header_t *head = chunks[0];
    chunk_dll_insert_sorted(&head, chunks[2]);
    chunk_dll_insert_sorted(&head, chunks[1]);

    TEST_ASSERT_EQUAL_PTR(head, chunks[0]);
    TEST_ASSERT_EQUAL_PTR(chunks[0]->next, chunks[1]);
    TEST_ASSERT_EQUAL_PTR(chunks[1]->next, chunks[2]);
    TEST_ASSERT_NULL(chunks[2]->next);
    TEST_ASSERT_EQUAL_PTR(chunks[2]->previous, chunks[1]);
    TEST_ASSERT_EQUAL_PTR(chunks[1]->previous, chunks[0]);
    TEST_ASSERT_NULL(chunks[0]->previous);

    free(chunk1);
    free(chunk2);
    free(chunk3);
}

static void test_insert_sorted_at_tail(void) {
    chunk_header_t *chunk1 = make_chunk(64);
    chunk_header_t *chunk2 = make_chunk(64);

    if (chunk2 < chunk1) {
        chunk_header_t *tmp = chunk1;
        chunk1 = chunk2;
        chunk2 = tmp;
    }
    chunk_header_t *head = chunk1;

    chunk_dll_insert_sorted(&head, chunk2);

    TEST_ASSERT_EQUAL_PTR(chunk1->next, chunk2);
    TEST_ASSERT_EQUAL_PTR(chunk2->previous, chunk1);
    TEST_ASSERT_NULL(chunk2->next);
    TEST_ASSERT_NULL(chunk1->previous);

    free(chunk1);
    free(chunk2);
}
