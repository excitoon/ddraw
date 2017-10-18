cmake_minimum_required (VERSION 3.0)

project (ddraw)

enable_testing ()

add_library (ddraw SHARED ddraw.cpp)
set_target_properties (ddraw PROPERTIES PREFIX "")
set_target_properties (ddraw PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (ddraw PROPERTIES LINK_FLAGS "-m32")

add_executable (hello hello.cpp)
set_target_properties (hello PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (hello PROPERTIES LINK_FLAGS "-m32")
add_test (hello hello)
