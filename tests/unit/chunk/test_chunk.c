#include "chunk.h"

#include <unity.h>

extern void run_chunk_dll_insert_tests(void);
extern void run_chunk_dll_insert_sorted_tests(void);
extern void run_chunk_dll_remove_tests(void);

void run_chunk_dll_tests(void) {
    run_chunk_dll_insert_tests();
    run_chunk_dll_insert_sorted_tests();
    run_chunk_dll_remove_tests();
}

chunk_header_t *make_chunk(size_t size) {
    chunk_header_t *chunk = malloc(sizeof(chunk_header_t));
    chunk->size = size;
    chunk->in_use = 0;
    chunk->next = NULL;
    chunk->previous = NULL;
    return chunk;
}
