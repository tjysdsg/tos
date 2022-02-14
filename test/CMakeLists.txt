macro(enable_tests)
    add_compile_definitions(__TOS_ENABLE_KERNEL_TESTS__)
endmacro()

set(KERNEL_TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/kernel_test.cpp CACHE STRING "Kernel test source files" FORCE)
