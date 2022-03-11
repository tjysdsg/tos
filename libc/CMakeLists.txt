add_library(
        c
        string.cpp
        stdio.cpp
)

target_compile_options(
        c
        PRIVATE -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -O2 -fno-threadsafe-statics
)
target_include_directories(c PUBLIC ${PROJECT_SOURCE_DIR}/libc)
