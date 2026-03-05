cmake_minimum_required(VERSION 3.20)
project(pspedit_common)





file(GLOB_RECURSE pspedit_common_sources "*.cpp")
add_library(pspedit_common STATIC ${pspedit_common_sources})
set_property(TARGET pspedit_common PROPERTY CXX_STANDARD 17)
target_include_directories(pspedit_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
# target_link_libraries(pspedit_common PRIVATE pspexploit pspdebug pspgu pspge pspdisplay pspctrl psprtc)
