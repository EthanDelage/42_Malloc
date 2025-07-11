cmake_minimum_required(VERSION 3.22.1)
project(malloc C)

set(CMAKE_C_STANDARD 11)

execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE MACHINE_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND uname -s
        OUTPUT_VARIABLE SYSTEM_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(DEFINED ENV{HOSTTYPE})
    set(HOST_TYPE "$ENV{HOSTTYPE}")
else()
    set(HOST_TYPE "${MACHINE_ARCH}_${SYSTEM_NAME}")
endif()
message(STATUS "Detected platform: ${HOST_TYPE}")
set(MALLOC_LIB_TARGET "ft_malloc_${HOST_TYPE}")
set(MALLOC_LIB_LINK_TARGET "libft_malloc${CMAKE_SHARED_LIBRARY_SUFFIX}")

add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(src)

option(BUILD_TESTS "Build the test suit" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unity)
    add_subdirectory(tests/integration)
    add_subdirectory(tests/unit)
endif ()
