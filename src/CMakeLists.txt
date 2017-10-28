cmake_minimum_required (VERSION 3.0)

project (ddraw)

enable_testing ()

include_directories (.)

add_definitions (-D_NTDDDISK_H_) # Avoiding redefinition of Unknown.
add_definitions (-DINITGUID)

set (RESOURCE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ddrawrc.o")
add_custom_command (OUTPUT "${RESOURCE_OUTPUT}"
        COMMAND windres -i "${CMAKE_CURRENT_SOURCE_DIR}/ddraw.rc" -o "${RESOURCE_OUTPUT}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ddraw.rc"
        COMMENT "Generating version info")
add_custom_target (resources DEPENDS "${RESOURCE_OUTPUT}")

add_library (ddraw SHARED ddraw.cpp "${RESOURCE_OUTPUT}")
add_dependencies (ddraw resources)
set_target_properties (ddraw PROPERTIES PREFIX "")
set_target_properties (ddraw PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (ddraw PROPERTIES LINK_FLAGS "-static -static-libstdc++ -static-libgcc -m32 -Wl,--kill-at")

add_subdirectory (tests)