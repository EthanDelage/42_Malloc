#include "chunk.h"

void chunk_dll_insert(chunk_header_t **pos, chunk_header_t *value) {
    chunk_header_t *current = *pos;
    chunk_header_t *next = NULL;

    if (current == NULL) {
        *pos = value;
        value->next = NULL;
        value->previous = NULL;
        return;
    }
    next = current->next;
    value->previous = current;
    value->next = next;
    current->next = value;
    if (next != NULL) {
        next->previous = value;
    }
}

void chunk_dll_remove(chunk_header_t **pos) {
    chunk_header_t *to_remove = *pos;
    chunk_header_t *previous = to_remove->previous;
    chunk_header_t *next = to_remove->next;

    if (next != NULL) {
        next->previous = previous;
        to_remove->next = NULL;
    }
    if (previous != NULL) {
        previous->next = next;
        to_remove->previous = NULL;
    } else {
        *pos = next;
    }
}