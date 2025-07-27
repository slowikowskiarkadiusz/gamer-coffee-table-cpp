cmake_minimum_required(VERSION 3.10)
project(gaming_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(gaming_coffee_table_cpp
        main.cpp
        engine/Engine.hpp
        engine/Actor.hpp
        engine/v2.hpp
        engine/Matrix.hpp
        engine/Color.hpp
        engine/Input/Input.hpp
        engine/Input/KeyboardInputProvider.hpp
        engine/Input/InputProvider.hpp
        engine/Input/Key.hpp
        engine/Input/KeyState.hpp
)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
