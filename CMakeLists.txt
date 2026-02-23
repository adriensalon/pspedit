cmake_minimum_required(VERSION 3.20)
project(pspedit)


set(BUILD_DWARFDUMP OFF)
add_subdirectory(external/libdwarf)
if(MSVC)
    # target_compile_definitions(C4090)
endif()

file(GLOB_RECURSE pspedit_editor_sources "source/editor/*.cpp")
add_executable(pspedit ${pspedit_editor_sources})
target_include_directories(pspedit PRIVATE "source")
target_link_libraries(pspedit PRIVATE dwarf)
set_target_properties(pspedit PROPERTIES CXX_STANDARD 17)