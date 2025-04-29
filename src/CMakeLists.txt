add_library(${MALLOC_LIB_TARGET} SHARED
        ${PROJECT_SOURCE_DIR}/include/malloc.h
        malloc.c
        free.c
        realloc.c)

add_custom_command(
        TARGET ${MALLOC_LIB_TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        $<TARGET_FILE:${MALLOC_LIB_TARGET}>
        $<TARGET_FILE_DIR:${MALLOC_LIB_TARGET}>/lib${MALLOC_LIB_LINK_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}
        COMMENT "Creating symlink lib${MALLOC_LIB_LINK_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX} -> ${MALLOC_LIB_TARGET}"
)

target_include_directories(${MALLOC_LIB_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
