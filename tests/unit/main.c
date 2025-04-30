#include <unity.h>

extern void run_chunk_dll_tests(void);

int main(void) {
    UNITY_BEGIN();
    run_chunk_dll_tests();
    return UNITY_END();
}
