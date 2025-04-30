add_executable(unit_tests
    main.c)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME run_unit_tests COMMAND unit_tests)