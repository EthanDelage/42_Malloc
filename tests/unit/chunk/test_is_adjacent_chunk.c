#include "test_chunk.h"

#include <stdlib.h>
#include <unity.h>

static void test_adjacent_chunks(void);
static void test_non_adjacent_chunks(void);
static void test_chunks_overlap_should_fail(void);

void run_is_adjacent_chunk_tests(void) {
    RUN_TEST(test_adjacent_chunks);
    RUN_TEST(test_non_adjacent_chunks);
    RUN_TEST(test_chunks_overlap_should_fail);
}

static void test_adjacent_chunks(void) {
    size_t size1 = 64;
    size_t size2 = 32;
    size_t total =
        sizeof(chunk_header_t) + size1 + sizeof(chunk_header_t) + size2;

    chunk_header_t *mem = malloc(total);
    chunk_header_t *first = mem;
    chunk_header_t *second =
        (chunk_header_t *)((char *)first + sizeof(chunk_header_t) + size1);

    first->size = size1;
    second->size = size2;

    TEST_ASSERT_EQUAL_UINT8(1, is_adjacent_chunk(first, second));

    free(mem);
}

static void test_non_adjacent_chunks(void) {
    size_t size1 = 64;
    size_t size2 = 32;
    size_t gap = 16;
    size_t total =
        sizeof(chunk_header_t) + size1 + gap + sizeof(chunk_header_t) + size2;

    chunk_header_t *mem = malloc(total);
    chunk_header_t *first = mem;
    chunk_header_t *second =
        (chunk_header_t *)((char *)first + sizeof(chunk_header_t) + size1 +
                           gap);

    first->size = size1;
    second->size = size2;

    TEST_ASSERT_EQUAL_UINT8(0, is_adjacent_chunk(first, second));

    free(mem);
}

static void test_chunks_overlap_should_fail(void) {
    size_t size1 = 64;
    size_t size2 = 128;

    // Allocate smaller buffer to force overlap
    size_t total = sizeof(chunk_header_t) + size1 + sizeof(chunk_header_t);
    chunk_header_t *mem = malloc(total);

    chunk_header_t *first = mem;
    chunk_header_t *second =
        (chunk_header_t *)((char *)first + sizeof(chunk_header_t) + size1 -
                           8); // intentionally overlap

    first->size = size1;
    second->size = size2;

    TEST_ASSERT_EQUAL_UINT8(0, is_adjacent_chunk(first, second));

    free(mem);
}
