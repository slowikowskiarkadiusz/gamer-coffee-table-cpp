cmake_minimum_required(VERSION 3.16)
project(gaming_coffee_table_cpp_desktop)
set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB_RECURSE SRC_FILES
        "../src/*.cpp"
        "../src/*.hpp"
        "../src/*.h"
)

add_executable(${PROJECT_NAME}
        main_desktop.cpp
        ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
target_include_directories(${PROJECT_NAME} PRIVATE ../src)