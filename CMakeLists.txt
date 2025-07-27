cmake_minimum_required(VERSION 3.10)
project(gaming_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(gaming_coffee_table_cpp ${SOURCES}
        engine/Engine.hpp
        engine/Actor.hpp
        engine/v2.cpp
        engine/v2.hpp
        engine/Matrix.cpp
        engine/Matrix.hpp
        engine/Color.hpp
)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
