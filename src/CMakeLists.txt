add_library(${MALLOC_LIB_TARGET} SHARED
        malloc.c
        free.c
        realloc.c
        chunk/chunk_dll.c
        chunk/chunk_search.c
        chunk/chunk_utils.c
        memory/memory_utils.c
        memory/memory_zone.c
        memory/page_header.c
        ${PROJECT_SOURCE_DIR}/include/malloc.h
        ${PROJECT_SOURCE_DIR}/include/memory/memory_utils.h
        ${PROJECT_SOURCE_DIR}/include/memory/memory_zone.h
        ${PROJECT_SOURCE_DIR}/include/memory/malloc_data.h
        ${PROJECT_SOURCE_DIR}/include/memory/page_header.h
        ${PROJECT_SOURCE_DIR}/include/memory/chunk_header.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_dll.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_utils.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_search.h)

add_custom_command(
        TARGET ${MALLOC_LIB_TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        $<TARGET_FILE:${MALLOC_LIB_TARGET}>
        $<TARGET_FILE_DIR:${MALLOC_LIB_TARGET}>/lib${MALLOC_LIB_LINK_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}
        COMMENT "Creating symlink lib${MALLOC_LIB_LINK_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX} -> ${MALLOC_LIB_TARGET}"
)

target_include_directories(${MALLOC_LIB_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(${MALLOC_LIB_TARGET} PRIVATE -fvisibility=hidden)
