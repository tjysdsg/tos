aux_source_directory(acpica/source/common ACPICA_SRCS)
aux_source_directory(acpica/source/compiler ACPICA_SRCS)
aux_source_directory(acpica/source/components ACPICA_SRCS)
# aux_source_directory(acpica/source/os_specific/efi ACPICA_SRCS)
aux_source_directory(acpica/source/os_specific/service_layers ACPICA_SRCS)

add_library(
        acpica
        ${ACPICA_SRCS}
)

target_compile_options(
        acpica
        PRIVATE -ffreestanding -Wall -Wextra -fno-exceptions
)
target_include_directories(acpica PUBLIC acpica/source/include acpica/source/compiler)
target_compile_definitions(acpica PUBLIC _TOS __KERNEL__)
target_link_libraries(acpica PUBLIC c)
