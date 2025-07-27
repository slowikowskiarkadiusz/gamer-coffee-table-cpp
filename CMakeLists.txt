cmake_minimum_required(VERSION 3.10)
project(gamer_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(led_matrix_sim ${SOURCES})
target_link_libraries(gamer_coffee_table_cpp sfml-graphics sfml-window sfml-system)
