# runtime_headers
add_library(runtime_headers INTERFACE)
target_include_directories(runtime_headers INTERFACE "../runtime")

# pspedit
file(GLOB_RECURSE pspedit_editor_sources "*.cpp")
add_executable(pspedit ${pspedit_editor_sources})
target_include_directories(pspedit PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(pspedit PRIVATE libusb dwarf glfw imgui nfd stb runtime_headers)
set_target_properties(pspedit PROPERTIES CXX_STANDARD 17)