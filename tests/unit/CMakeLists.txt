add_executable(unit_tests
    main.c
    test_chunk_dll.c
    ${PROJECT_SOURCE_DIR}/src/chunk/chunk_dll.c)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(unit_tests unity)

add_test(NAME run_unit_tests COMMAND unit_tests)