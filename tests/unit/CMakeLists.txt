add_executable(unit_tests
    ${PROJECT_SOURCE_DIR}/src/chunk/chunk_dll.c
    ${PROJECT_SOURCE_DIR}/src/chunk/chunk_utils.c
    main.c
    chunk/test_chunk.c
    chunk/test_chunk_dll_remove.c
    chunk/test_chunk.h
    chunk/test_chunk_dll_insert.c
    chunk/test_chunk_dll_insert_sorted.c
    chunk/test_chunk_split.c
    chunk/test_chunk_coalesce.c
    chunk/test_is_adjacent_chunk.c
    chunk/test_chunk_dll_transfer_sorted.c)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(unit_tests unity)

add_test(NAME run_unit_tests COMMAND unit_tests)