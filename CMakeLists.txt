cmake_minimum_required(VERSION 3.20)
project(pspedit)

file(GLOB_RECURSE pspedit_editor_sources "source/editor/*.cpp")
add_executable(pspedit ${pspedit_editor_sources} ".dev/main.cpp")
target_include_directories(pspedit PRIVATE "source")
set_target_properties(pspedit PROPERTIES CXX_STANDARD 17)