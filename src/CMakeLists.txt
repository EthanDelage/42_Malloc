add_library(${MALLOC_LIB_TARGET} SHARED
        ${PROJECT_SOURCE_DIR}/include/malloc.h
        malloc.c
        free.c
        realloc.c)

target_include_directories(${MALLOC_LIB_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
