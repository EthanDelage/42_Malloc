add_executable(malloc_tests
    main.c)

target_link_libraries(malloc_tests PRIVATE ${MALLOC_LIB_TARGET})

add_test(NAME run_integration_tests COMMAND malloc_tests)