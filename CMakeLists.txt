cmake_minimum_required(VERSION 3.25)
project(te)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(te main.cpp document.h document.cpp editor.cpp editor.h)
target_link_libraries(te ${CURSES_LIBRARIES})
