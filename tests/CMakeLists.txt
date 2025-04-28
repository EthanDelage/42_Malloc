add_executable(test_malloc
    main.c)

target_link_libraries(test_malloc PRIVATE ${MALLOC_LIB_TARGET})