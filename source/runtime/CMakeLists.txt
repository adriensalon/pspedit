cmake_minimum_required(VERSION 3.20)
project(pspedit_runtime)





file(GLOB_RECURSE pspedit_runtime_sources "*.cpp")
add_executable(pspedit_runtime ${pspedit_runtime_sources})
set_property(TARGET pspedit_runtime PROPERTY CXX_STANDARD 17)
target_include_directories(pspedit_runtime PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pspedit_runtime PRIVATE pspexploit pspdebug pspgu pspge pspdisplay pspctrl psprtc)

create_pbp_file(
    TARGET pspedit_runtime
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE pspedit_runtime
    VERSION 01.00)