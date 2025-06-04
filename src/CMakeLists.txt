add_library(${MALLOC_LIB_TARGET} SHARED
        malloc.c
        free.c
        realloc.c
        calloc.c
        print.c
        chunk/chunk_dll.c
        chunk/chunk_search.c
        chunk/chunk_utils.c
        memory/memory_utils.c
        memory/memory_zone.c
        memory/page_header.c
        utils/printf.c
        ${PROJECT_SOURCE_DIR}/include/malloc.h
        ${PROJECT_SOURCE_DIR}/include/memory/memory_utils.h
        ${PROJECT_SOURCE_DIR}/include/memory/memory_zone.h
        ${PROJECT_SOURCE_DIR}/include/memory/malloc_data.h
        ${PROJECT_SOURCE_DIR}/include/memory/page_header.h
        ${PROJECT_SOURCE_DIR}/include/memory/chunk_header.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_dll.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_utils.h
        ${PROJECT_SOURCE_DIR}/include/chunk/chunk_search.h
        ${PROJECT_SOURCE_DIR}/include/utils/printf.h)

add_custom_command(
        TARGET ${MALLOC_LIB_TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        $<TARGET_FILE:${MALLOC_LIB_TARGET}>
        $<TARGET_FILE_DIR:${MALLOC_LIB_TARGET}>/${MALLOC_LIB_LINK_TARGET}
        COMMENT "Creating symlink ${MALLOC_LIB_LINK_TARGET} -> ${MALLOC_LIB_TARGET}"
)

set_directory_properties(PROPERTIES
        ADDITIONAL_MAKE_CLEAN_FILES
            $<TARGET_FILE_DIR:${MALLOC_LIB_TARGET}>/${MALLOC_LIB_LINK_TARGET}
)

target_include_directories(${MALLOC_LIB_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
#target_compile_options(${MALLOC_LIB_TARGET} PRIVATE -fvisibility=hidden)
