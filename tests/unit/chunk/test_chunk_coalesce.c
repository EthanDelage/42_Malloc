#include "test_chunk.h"

#include <stdlib.h>
#include <unity.h>

static void test_chunk_coalesce_adjacent_chunks(void);
static void test_chunk_coalesce_updates_links(void);
static void test_chunk_coalesce_size_update(void);

void run_chunk_coalesce_tests(void) {
    RUN_TEST(test_chunk_coalesce_adjacent_chunks);
    RUN_TEST(test_chunk_coalesce_updates_links);
    RUN_TEST(test_chunk_coalesce_size_update);
}

static void test_chunk_coalesce_adjacent_chunks(void) {
    size_t size1 = 64;
    size_t size2 = 128;
    size_t total =
        sizeof(chunk_header_t) + size1 + sizeof(chunk_header_t) + size2;

    chunk_header_t *mem = malloc(total);

    chunk_header_t *chunk = mem;
    chunk_header_t *next =
        (chunk_header_t *)((char *)chunk + sizeof(chunk_header_t) + size1);

    *chunk = (chunk_header_t){
        .size = size1, .in_use = 0, .next = next, .previous = NULL};
    *next = (chunk_header_t){
        .size = size2, .in_use = 0, .next = NULL, .previous = chunk};

    chunk_coalesce(chunk);

    TEST_ASSERT_EQUAL(size1 + sizeof(chunk_header_t) + size2, chunk->size);
    TEST_ASSERT_NULL(chunk->next);

    free(mem);
}

static void test_chunk_coalesce_updates_links(void) {
    size_t size1 = 64;
    size_t size2 = 32;
    size_t size3 = 16;
    size_t total = sizeof(chunk_header_t) * 3 + size1 + size2 + size3;

    chunk_header_t *mem = malloc(total);

    chunk_header_t *chunk = mem;
    chunk_header_t *next =
        (chunk_header_t *)((char *)chunk + sizeof(chunk_header_t) + size1);
    chunk_header_t *after =
        (chunk_header_t *)((char *)next + sizeof(chunk_header_t) + size2);

    *chunk = (chunk_header_t){
        .size = size1, .in_use = 0, .next = next, .previous = NULL};
    *next = (chunk_header_t){
        .size = size2, .in_use = 0, .next = after, .previous = chunk};
    *after = (chunk_header_t){
        .size = size3, .in_use = 0, .next = NULL, .previous = next};

    chunk_coalesce(chunk);

    TEST_ASSERT_EQUAL_PTR(after, chunk->next);
    TEST_ASSERT_EQUAL_PTR(chunk, after->previous);

    free(mem);
}

static void test_chunk_coalesce_size_update(void) {
    size_t size1 = 80;
    size_t size2 = 48;
    size_t total =
        sizeof(chunk_header_t) + size1 + sizeof(chunk_header_t) + size2;

    chunk_header_t *mem = malloc(total);

    chunk_header_t *chunk = mem;
    chunk_header_t *next =
        (chunk_header_t *)((char *)chunk + sizeof(chunk_header_t) + size1);

    *chunk = (chunk_header_t){
        .size = size1, .in_use = 0, .next = next, .previous = NULL};
    *next = (chunk_header_t){
        .size = size2, .in_use = 0, .next = NULL, .previous = chunk};

    chunk_coalesce(chunk);

    TEST_ASSERT_EQUAL(size1 + sizeof(chunk_header_t) + size2, chunk->size);

    free(mem);
}
