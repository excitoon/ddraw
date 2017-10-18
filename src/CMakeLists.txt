project (ddraw)

add_library (ddraw SHARED ddraw.cpp)
set_target_properties (ddraw PROPERTIES PREFIX "")
set_target_properties (ddraw PROPERTIES COMPILE_FLAGS "-m32")
set_target_properties (ddraw PROPERTIES LINK_FLAGS "-m32")