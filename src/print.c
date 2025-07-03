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

__attribute__((visibility("default")))
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
}

static void print_chunk_dll(chunk_header_t *chunk) {
    void *ptr;

    while (chunk != NULL) {
        ptr = get_chunk_data(chunk);
        printf("0x%p -> 0x%p - 0x%p: % 8zu bytes (status: %d)\n", chunk, ptr, ptr + chunk->size - 1, chunk->size, chunk->in_use);
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
