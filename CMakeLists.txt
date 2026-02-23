cmake_minimum_required(VERSION 3.20)
project(pspedit)



add_subdirectory(external/libdwarf)
if(MSVC)
    # target_compile_definitions(C4090)
endif()

# glfw
set(glfw_main_sources
    "external/glfw/src/context.c"
    "external/glfw/src/init.c"
    "external/glfw/src/input.c"
    "external/glfw/src/monitor.c"
    "external/glfw/src/platform.c"
    "external/glfw/src/vulkan.c"
    "external/glfw/src/window.c"
    "external/glfw/src/egl_context.c"
    "external/glfw/src/osmesa_context.c"
    "external/glfw/src/null_init.c"
    "external/glfw/src/null_monitor.c"
    "external/glfw/src/null_window.c"
    "external/glfw/src/null_joystick.c")
if(APPLE)
    set(glfw_platform_sources
        "external/glfw/src/cocoa_time.c"
        "external/glfw/src/posix_module.c"
        "external/glfw/src/posix_thread.c")
elseif(WIN32)
    set(glfw_platform_sources
        "external/glfw/src/win32_time.c"
        "external/glfw/src/win32_module.c"
        "external/glfw/src/win32_thread.c"
        "external/glfw/src/win32_init.c"
        "external/glfw/src/win32_joystick.c"
        "external/glfw/src/win32_monitor.c"
        "external/glfw/src/win32_window.c"
        "external/glfw/src/wgl_context.c")    
else()
    set(glfw_platform_sources 
        "external/glfw/src/posix_module.c"
        "external/glfw/src/posix_time.c"
        "external/glfw/src/posix_thread.c")
endif()
set(glfw_sources ${glfw_main_sources} ${glfw_platform_sources})
add_library(glfw STATIC EXCLUDE_FROM_ALL ${glfw_sources})
set_target_properties(glfw PROPERTIES CXX_STANDARD 17)
target_include_directories(glfw PUBLIC "external/glfw/include")
target_include_directories(glfw PUBLIC "external/glfw/deps")
if(WIN32)
    target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(glfw PUBLIC OpenGL::GL)
endif()
if(MSVC)
    target_compile_options(glfw PRIVATE /wd4034 /wd4996)
endif()

# imgui
set(imgui_sources
	"external/imgui/imgui.cpp"
	"external/imgui/imgui_widgets.cpp"
	"external/imgui/imgui_tables.cpp"
	"external/imgui/imgui_draw.cpp"
	"external/imgui/imgui_demo.cpp"
	"external/imgui/misc/cpp/imgui_stdlib.cpp"	
	"external/imgui/backends/imgui_impl_opengl3.cpp"
    "external/imgui/backends/imgui_impl_glfw.cpp")
add_library(imgui STATIC EXCLUDE_FROM_ALL ${imgui_sources})
set_target_properties(imgui PROPERTIES CXX_STANDARD 17)
target_compile_definitions(imgui PUBLIC -DIMGUI_USE_WCHAR32)
target_include_directories(imgui PUBLIC "external/imgui")
target_include_directories(imgui PUBLIC "external/imgui/backends")
target_link_libraries(imgui PRIVATE glfw)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "external/stb")

# assimp

# pspeditrt
add_library(pspeditrt INTERFACE)
target_include_directories(pspeditrt INTERFACE "external/pspeditrt/include")

file(GLOB_RECURSE pspedit_editor_sources "source/*.cpp")
add_executable(pspedit ${pspedit_editor_sources})
target_include_directories(pspedit PRIVATE "source")
target_link_libraries(pspedit PRIVATE dwarf glfw imgui stb pspeditrt)
set_target_properties(pspedit PROPERTIES CXX_STANDARD 17)