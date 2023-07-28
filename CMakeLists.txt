cmake_minimum_required(VERSION 3.25)
project(te)

set(CMAKE_CXX_STANDARD 17)

find_library(NCURSESW_LIBRARY NAMES ncursesw)
include_directories(/usr/include) # Path to ncursesw .h files

add_executable(te main.cpp document.h document.cpp editor.cpp editor.h utf8.cpp utf8.h)
target_link_libraries(te ${NCURSESW_LIBRARY})
