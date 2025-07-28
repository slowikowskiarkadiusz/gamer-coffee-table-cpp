cmake_minimum_required(VERSION 3.10)
project(gaming_coffee_table_cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(gaming_coffee_table_cpp
        main.cpp
        engine/engine.hpp
        engine/v2.hpp
        engine/matrix.hpp
        engine/color.hpp
        engine/input/input.hpp
        engine/input/keyboard_input_provider.hpp
        engine/input/input_provider.hpp
        engine/input/key.hpp
        engine/input/key_state.hpp
        engine/scene.hpp
        engine/scenes/menu/menu-scene.hpp
        engine/actors/rectangle-actor.hpp
        engine/actors/arrow-actor.hpp
        engine/actor.hpp
        engine/actor.cpp
        engine/engine.cpp
        engine/scenes/menu/menu-scene.cpp
        engine/actors/rectangle-actor.cpp
)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
