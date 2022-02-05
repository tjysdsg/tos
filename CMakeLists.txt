cmake_minimum_required(VERSION 3.16)
project(tos C CXX ASM)

add_library(boot OBJECT kernel/boot.S)

add_library(kernel OBJECT kernel/kernel.cpp kernel/FrameBufferFontWriter.cpp kernel/FrameBuffer.cpp)
target_compile_options(kernel PRIVATE -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti)

# generate multiboot-compliant kernel file
set(MULTIBOOT_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tos.bin)
set(ISO_TMP_DIR ${CMAKE_BINARY_DIR}/isodir)
set(ISO_PATH ${CMAKE_BINARY_DIR}/tos.iso)
add_custom_command(
        OUTPUT ${MULTIBOOT_OUTPUT_PATH} ${ISO_PATH}

        # link the kernel
        COMMAND echo "Linking $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel> into a kernel file"
        COMMAND ${CMAKE_C_COMPILER} -T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -o ${MULTIBOOT_OUTPUT_PATH} -ffreestanding -nostdlib $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel> -lgcc
        # create grub image
        COMMAND mkdir -p ${ISO_TMP_DIR}/boot/grub
        COMMAND cp ${MULTIBOOT_OUTPUT_PATH} ${ISO_TMP_DIR}/boot/
        COMMAND cp ${PROJECT_SOURCE_DIR}/kernel/grub.cfg ${ISO_TMP_DIR}/boot/grub/grub.cfg
        COMMAND grub-mkrescue -o ${ISO_PATH} ${ISO_TMP_DIR}

        DEPENDS boot kernel ${PROJECT_SOURCE_DIR}/kernel/linker.ld ${PROJECT_SOURCE_DIR}/kernel/grub.cfg $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel>

        COMMAND_EXPAND_LISTS
)
add_custom_target(multiboot ALL DEPENDS ${MULTIBOOT_OUTPUT_PATH})

# shortcut for running qemu with the multiboot kernel
add_custom_target(qemu-multiboot
        # COMMAND qemu-system-i386 -kernel ${MULTIBOOT_OUTPUT_PATH}
        COMMAND qemu-system-i386 -cdrom ${ISO_PATH}
        DEPENDS multiboot
        )
