cmake_minimum_required(VERSION 3.16)
project(tos CXX ASM)

add_library(boot OBJECT boot/boot.S)

add_library(kernel OBJECT kernel/kernel.cpp)
target_compile_options(kernel PRIVATE -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti)

# generate multiboot-compliant kernel file
set(MULTIBOOT_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tos.bin)
add_custom_command(OUTPUT ${MULTIBOOT_OUTPUT_PATH}
        COMMAND echo "Linking $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel> into a kernel file"
        COMMAND ${CMAKE_C_COMPILER} -T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -o ${MULTIBOOT_OUTPUT_PATH} -ffreestanding -nostdlib $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel> -lgcc
        DEPENDS boot kernel ${PROJECT_SOURCE_DIR}/kernel/linker.ld
        )
add_custom_target(multiboot ALL DEPENDS ${MULTIBOOT_OUTPUT_PATH})

# shortcut for running qemu with the multiboot kernel
add_custom_target(qemu-multiboot
        COMMAND qemu-system-i386 -kernel ${MULTIBOOT_OUTPUT_PATH}
        DEPENDS multiboot
        )
