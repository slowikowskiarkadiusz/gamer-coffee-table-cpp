cmake_minimum_required(VERSION 3.10)
project(gaming_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(gaming_coffee_table_cpp ${SOURCES}
        engine/engine.cpp
        engine/Engine.h
        engine/Actor.cpp
        engine/Actor.h
        engine/v2.cpp
        engine/v2.h)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
