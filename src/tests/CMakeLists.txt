string(REPLACE "\\" "/" SYSTEM_ROOT "$ENV{SystemRoot}")

add_executable (scheduler scheduler.cpp)
set_target_properties (scheduler PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (scheduler PROPERTIES LINK_FLAGS "-m32")
add_test (scheduler scheduler)
set_tests_properties (scheduler PROPERTIES TIMEOUT 1)

add_executable (hello hello.cpp)
set_target_properties (hello PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (hello PROPERTIES LINK_FLAGS "-m32")
add_test (hello hello "${CMAKE_BINARY_DIR}/ddraw.dll")
set_tests_properties (hello PROPERTIES TIMEOUT 1)

add_executable (simple simple.cpp)
set_target_properties (simple PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (simple PROPERTIES LINK_FLAGS "-m32")
add_test (simple simple "${CMAKE_BINARY_DIR}/ddraw.dll")
set_tests_properties (simple PROPERTIES TIMEOUT 1)

add_test (simple_original simple "${SYSTEM_ROOT}/System32/ddraw.dll")
set_tests_properties (simple_original PROPERTIES TIMEOUT 1)
