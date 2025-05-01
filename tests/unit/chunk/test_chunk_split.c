#include "test_chunk.h"

#include <unity.h>

#define MIN_SPLIT_REMAINDER (sizeof(chunk_header_t) + ALIGNMENT)

static void test_chunk_split_success(void);
static void test_chunk_split_exact_fit(void);
static void test_chunk_split_too_small_to_split(void);
static void test_chunk_split_pointer_links(void);

void run_chunk_split_tests(void) {
    RUN_TEST(test_chunk_split_success);
    RUN_TEST(test_chunk_split_exact_fit);
    RUN_TEST(test_chunk_split_too_small_to_split);
    RUN_TEST(test_chunk_split_pointer_links);
}

static void test_chunk_split_success(void) {
    size_t original_size = 128;
    size_t request_size = 64;
    chunk_header_t *chunk = make_chunk(original_size);

    chunk_header_t *second = chunk_split(chunk, request_size);

    TEST_ASSERT_NOT_NULL(second);
    TEST_ASSERT_EQUAL(request_size, chunk->size);
    TEST_ASSERT_EQUAL(original_size - sizeof(chunk_header_t) - request_size,
                      second->size);
    TEST_ASSERT_EQUAL_PTR(chunk->next, second);
    TEST_ASSERT_EQUAL_PTR(second->previous, chunk);

    free(chunk);
}

static void test_chunk_split_exact_fit(void) {
    size_t request_size = 120;
    size_t original_size = request_size + sizeof(chunk_header_t);
    chunk_header_t *chunk = make_chunk(original_size);

    chunk_header_t *second = chunk_split(chunk, request_size);

    TEST_ASSERT_NULL(second);
    TEST_ASSERT_EQUAL(original_size, chunk->size);
    TEST_ASSERT_NULL(chunk->next);

    free(chunk);
}

static void test_chunk_split_too_small_to_split(void) {
    size_t request_size = 120;
    size_t original_size = request_size + MIN_SPLIT_REMAINDER - 1;
    chunk_header_t *chunk = make_chunk(original_size);

    chunk_header_t *second = chunk_split(chunk, request_size);

    TEST_ASSERT_NULL(second);
    TEST_ASSERT_EQUAL(original_size, chunk->size);
    TEST_ASSERT_NULL(chunk->next);

    free(chunk);
}

static void test_chunk_split_pointer_links(void) {
    chunk_header_t *chunk = make_chunk(200);
    chunk_header_t *after = make_chunk(32);
    chunk->next = after;
    after->previous = chunk;

    chunk_header_t *second = chunk_split(chunk, 64);

    TEST_ASSERT_NOT_NULL(second);
    TEST_ASSERT_EQUAL_PTR(chunk->next, second);
    TEST_ASSERT_EQUAL_PTR(second->previous, chunk);
    TEST_ASSERT_EQUAL_PTR(second->next, after);
    TEST_ASSERT_EQUAL_PTR(after->previous, second);

    free(after);
    free(chunk);
}
