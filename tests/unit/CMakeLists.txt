add_executable(unit_tests
    ${PROJECT_SOURCE_DIR}/src/chunk/chunk_dll.c
    main.c
    chunk/test_chunk.c
    chunk/test_chunk_dll_remove.c
    chunk/test_chunk.h
    chunk/test_chunk_dll_insert.c)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(unit_tests unity)

add_test(NAME run_unit_tests COMMAND unit_tests)