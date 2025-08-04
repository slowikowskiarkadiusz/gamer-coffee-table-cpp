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
        src/scenes/menu/menu_scene.hpp
        src/actors/rectangle_actor.hpp
        src/actors/arrow_actor.hpp
        src/actor.hpp
        src/actor.cpp
        src/engine.cpp
        src/scenes/menu/menu_scene.cpp
        src/actors/rectangle_actor.cpp
        src/actors/arrow_actor.cpp
        src/actors/text_actor.cpp
        src/actors/text_actor.hpp
        src/actors/letter.cpp
        src/actors/letter.hpp
        src/scenes/tetris/tetris_scene.cpp
        src/scenes/tetris/tetris_scene.hpp
        src/scenes/pong/pong_scene.cpp
        src/scenes/pong/pong_scene.hpp
        src/input/input.cpp
        src/input/gestures.hpp
        src/input/gestures.cpp
        src/max_heap.hpp
        src/now_ms.hpp
        src/scenes/controls/controls_scene.hpp
        src/scenes/controls/button_icon_actor.hpp
        src/scenes/controls/controls_scene.cpp
        src/scenes/tetris/tetris_block.hpp
        src/scenes/tetris/tetris_shape.hpp
        src/scenes/tetris/tetris_block_colors.cpp
        src/scenes/tetris/garbage_bar_logic.hpp
        src/scenes/tetris/hold_logic.hpp
        src/scenes/tetris/tetris_board_actor.cpp
        src/scenes/tetris/tetris_board_actor.h
        src/coroutine/task.hpp
        src/coroutine/wait_for_ms.hpp
        src/scenes/tetris/hold_logic.cpp
        src/scenes/snake/snake_scene.cpp
        src/scenes/snake/snake_scene.h
        src/scenes/tanks/tanks_scene.cpp
        src/scenes/tanks/tanks_scene.hpp
        src/actors/border_actor.cpp
        src/actors/border_actor.hpp
)
target_link_libraries(gaming_coffee_table_cpp sfml-graphics sfml-window sfml-system)
