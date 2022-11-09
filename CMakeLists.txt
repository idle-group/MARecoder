cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(MARecorder LANGUAGES CXX)
include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(src)
include_directories(src/windows)
add_library(init_window
    STATIC
        src/init_window.h
        src/init_window.cpp
)
add_library(windows 
    STATIC
        src/windows/window_base.h
        src/windows/choose_number_window.cpp
)
add_library(backends
    STATIC
        src/backends.h
        src/backends.cpp
)
add_library(imgui_impl_glfw
    STATIC 
    lib/imgui/backends/imgui_impl_glfw.h
    lib/imgui/backends/imgui_impl_glfw.cpp
)
add_library(imgui_impl_opengl3
    STATIC
    lib/imgui/backends/imgui_impl_opengl3.h
    lib/imgui/backends/imgui_impl_opengl3.cpp
)


add_library(imgui
    STATIC
    lib/imgui/imgui.h
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    #lib/imgui/imgui_demo.cpp
)


add_executable(MARecorder_glfw_opengl src/main.cpp)

target_link_libraries(MARecorder_glfw_opengl init_window windows backends imgui imgui_impl_glfw imgui_impl_opengl3 libglfw.so.3 libGL.so)