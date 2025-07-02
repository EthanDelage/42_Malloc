#include "chunk/chunk_utils.h"
#include "memory/malloc_data.h"
#include "memory/page_header.h"
#include "utils/printf.h"

static void print_normal_zone(page_header_t *head, const char *zone_str);
static void print_large_zone();
static void print_page(page_header_t *page, const char *zone_str, size_t index);
static void print_chunk_dll(chunk_header_t *chunk);
static size_t get_allocated_size();
static size_t get_normal_zone_size(page_header_t *head);
static size_t get_chunk_dll_size(chunk_header_t *head);
static void hexdump(const void *data, size_t size);

void show_alloc_mem(void) {
    print_normal_zone(malloc_data.tiny, "TINY");
    print_normal_zone(malloc_data.small, "SMALL");
    print_large_zone();
    printf("Total: %zu\n", get_allocated_size());
}

static void print_normal_zone(page_header_t *head, const char *zone_str) {
    size_t index = 0;

    while (head != NULL) {
        print_page(head, zone_str, index);
        index++;
        head = head->next;
    }
}

static void print_large_zone() {
    if (malloc_data.large != NULL) {
        printf("LARGE:\n");
        print_chunk_dll(malloc_data.large);
    }
}

static void print_page(page_header_t *page, const char *zone_str,
                       size_t index) {
    printf("%s (%zu): 0x%p - 0x%p\n", zone_str, index, page, ((char *)page) + page->size );
    print_chunk_dll(page->alloc_list);
    hexdump(page, page->size);
    printf("end\n");
}

static void print_chunk_dll(chunk_header_t *chunk) {
    void *ptr;

    while (chunk != NULL) {
        ptr = get_chunk_data(chunk);
        printf("0x%p -> 0x%p - 0x%p: % 8zu bytes (status: %d)\n", chunk, ptr, ptr + chunk->size - 1, chunk->size, chunk->in_use);
        hexdump(chunk, sizeof(chunk_header_t));
        chunk = chunk->next;
    }
}

static size_t get_allocated_size() {
    return get_normal_zone_size(malloc_data.tiny) +
           get_normal_zone_size(malloc_data.small) +
           get_chunk_dll_size(malloc_data.large);
}

static size_t get_normal_zone_size(page_header_t *head) {
    size_t size = 0;

    while (head != NULL) {
        size += get_chunk_dll_size(head->alloc_list);
        head = head->next;
    }
    return size;
}

static size_t get_chunk_dll_size(chunk_header_t *head) {
    size_t size = 0;

    while (head != NULL) {
        size += head->size;
        head = head->next;
    }
    return size;
}

#include <ctype.h>

static void hexdump(const void *data, size_t size) {
    const unsigned char *byte = (const unsigned char *)data;
    size_t i, j;

    for (i = 0; i < size; i += 16) {
        // Print offset
        printf("%p %08zx  ", data + size, i);

        // Print hex bytes
        for (j = 0; j < 16; j++) {
            if (i + j < size)
                printf("%02x ", byte[i + j]);
            else
                printf("   "); // padding for incomplete line
            if (j == 7)
                printf(" "); // extra space in the middle
        }

        // Print ASCII representation
        printf(" |");
        for (j = 0; j < 16 && i + j < size; j++) {
            unsigned char c = byte[i + j];
            printf("%c", isprint(c) ? c : '.');
        }
        printf("|\n");
    }
}

