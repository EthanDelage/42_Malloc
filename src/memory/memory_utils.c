#include "memory/memory_utils.h"
#include "chunk/chunk_utils.h"

#include <sys/mman.h>
#include <unistd.h>

/**
 * @brief Rounds up a given size to the nearest multiple of the system page
 * size.
 *
 * Useful when allocating memory with `mmap`, which requires page-aligned sizes.
 * @param size The original size in bytes.
 * @return The smallest multiple of the system page size that is ≥ size.
 */
size_t align_page(size_t size) {
    size_t page = sysconf(_SC_PAGESIZE);
    return (size + page - 1) & ~(page - 1);
}

size_t align_size(size_t size) {
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

/**
 * @brief Maps a region of memory with the specified size.
 *
 * This function uses the `mmap` system call to map a region of memory into the
 * process's address space. The region is marked as both readable and writable.
 * The size of the region must be a multiple of the system's page size.
 *
 * The memory region is allocated anonymously and is not backed by any file.
 * The region is private to the process, meaning changes to the region are not
 * visible to other processes.
 *
 * @param size The size of the memory region to be mapped. This must be a
 * multiple of `sysconf(_SC_PAGESIZE)` (the system's page size).
 * @return A pointer to the beginning of the mapped memory region, or
 * `MAP_FAILED` if the mapping fails.
 */
void *map_heap_region(size_t size) {
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
                -1, 0);
}

/**
 * @brief Unmaps a previously mapped region of memory.
 *
 * This function uses the `munmap` system call to unmap a region of memory that
 * was previously mapped.
 *
 * The size must match the value used when the region was mapped. After the
 * call, accessing the memory region is undefined behavior.
 *
 * @param addr A pointer to the beginning of the memory region to unmap.
 * @param size The size of the memory region to unmap. This must be the same
 * size that was passed to `mmap`, and must be a multiple of
 * `sysconf(_SC_PAGESIZE)`.
 * @return 0 on success, or -1 if the unmapping fails (errno is set
 * appropriately).
 */
int unmap_heap_region(void *addr, size_t size) { return munmap(addr, size); }
