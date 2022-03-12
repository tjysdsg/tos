cmake_minimum_required(VERSION 3.16)
project(tos C CXX ASM_NASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(ENABLE_TESTS "Enable tests" ON)

include_directories(thirdparty thirdparty/acpica/source/include test .)

add_subdirectory(test)
message(STATUS "Kernel test sources ${KERNEL_TEST_SOURCES}")

if (${ENABLE_TESTS})
    enable_tests()
endif ()

add_subdirectory(libc)
add_subdirectory(thirdparty)

add_library(
        boot OBJECT
        kernel/boot.asm
        kernel/gdt.asm
        kernel/idt.asm
        kernel/paging.asm
)

add_library(
        kernel STATIC

        kernel/kernel.cpp
        kernel/kprintf.cpp
        kernel/kpanic.cpp
        kernel/port.cpp
        kernel/pit.cpp
        kernel/tty.cpp
        kernel/idt.cpp
        kernel/isr.cpp
        kernel/paging.cpp
        kernel/memory.cpp
        kernel/cpuid.cpp
        kernel/pic.cpp
        kernel/ps2_keyboard.cpp
        kernel/VBEConsoleDisplay.cpp
        kernel/VGAConsoleDisplay.cpp
        kernel/cxxabi.cpp
        kernel/acpi_driver.cpp
        kernel/Timer.cpp
        kernel/PITTimer.cpp

        ${KERNEL_TEST_SOURCES}
)
target_compile_options(
        kernel
        PRIVATE -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -O2 -fno-threadsafe-statics
)
target_link_libraries(kernel PUBLIC acpica c gcc)

# prepare ssfn font file for linking with the kernel
set(FONT_O_PATH ${CMAKE_BINARY_DIR}/ssfn.o)
set(FONT_PATH ${PROJECT_SOURCE_DIR}/data/font/unifont.sfn)
add_custom_command(
        OUTPUT ${FONT_O_PATH}
        COMMAND cp ${FONT_PATH} font.sfn
        COMMAND echo Creating ssfn.o: ${TOOLCHAIN_LINKER} -r -b binary -o ${FONT_O_PATH} font.sfn
        COMMAND ${TOOLCHAIN_LINKER} -r -b binary -o ${FONT_O_PATH} font.sfn
        DEPENDS ${FONT_PATH}
)
add_custom_target(font ALL DEPENDS ${FONT_PATH})

# generate multiboot-compliant kernel file
set(MULTIBOOT_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tos.bin)
set(ISO_TMP_DIR ${CMAKE_BINARY_DIR}/isodir)
set(ISO_PATH ${CMAKE_BINARY_DIR}/tos.iso)
add_custom_command(
        OUTPUT ${MULTIBOOT_OUTPUT_PATH} ${ISO_PATH}

        # link the kernel
        COMMAND echo "Linking $<TARGET_OBJECTS:boot> $<TARGET_FILE:kernel> $<TARGET_FILE:acpica> into a kernel file"
        COMMAND ${CMAKE_C_COMPILER} -T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -o ${MULTIBOOT_OUTPUT_PATH} -ffreestanding -nostdlib $<TARGET_OBJECTS:boot> $<TARGET_FILE:kernel> $<TARGET_FILE:acpica> $<TARGET_FILE:c> ${FONT_O_PATH} -lgcc
        # create grub image
        COMMAND mkdir -p ${ISO_TMP_DIR}/boot/grub
        COMMAND cp ${MULTIBOOT_OUTPUT_PATH} ${ISO_TMP_DIR}/boot/
        COMMAND cp ${PROJECT_SOURCE_DIR}/kernel/grub.cfg ${ISO_TMP_DIR}/boot/grub/grub.cfg
        COMMAND grub-mkrescue -o ${ISO_PATH} ${ISO_TMP_DIR}

        DEPENDS boot kernel c acpica ${PROJECT_SOURCE_DIR}/kernel/linker.ld ${PROJECT_SOURCE_DIR}/kernel/grub.cfg $<TARGET_OBJECTS:boot> font ${FONT_O_PATH}
        COMMAND_EXPAND_LISTS
)
add_custom_target(multiboot ALL DEPENDS ${MULTIBOOT_OUTPUT_PATH})

# shortcut for running qemu with the multiboot kernel
add_custom_target(qemu-multiboot
        # COMMAND qemu-system-i386 -kernel ${MULTIBOOT_OUTPUT_PATH}
        COMMAND qemu-system-i386 -cdrom ${ISO_PATH}
        DEPENDS multiboot
        )
