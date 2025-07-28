cmake_minimum_required(VERSION 3.10)
project(gaming_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(gaming_coffee_table_cpp
        main.cpp
        src/engine.hpp
        src/v2.hpp
        src/matrix.hpp
        src/color.hpp
        src/input/input.hpp
        src/input/keyboard_input_provider.hpp
        src/input/input_provider.hpp
        src/input/key.hpp
        src/input/key_state.hpp
        src/scene.hpp
        src/scenes/menu/menu-scene.hpp
        src/actors/rectangle-actor.hpp
        src/actors/arrow-actor.hpp
        src/actor.hpp
        src/actor.cpp
        src/engine.cpp
        src/scenes/menu/menu-scene.cpp
        src/actors/rectangle-actor.cpp
)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
