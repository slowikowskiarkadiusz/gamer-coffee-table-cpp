cmake_minimum_required(VERSION 3.16)
project(gaming_coffee_table_cpp)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_DESKTOP "Build for desktop with SFML" ON)
option(BUILD_ESP32   "Build for ESP32 with ESP-IDF" OFF)

# katalogi build
set(DESKTOP_BLD ${CMAKE_SOURCE_DIR}/build_desktop)
set(ESP32_BLD   ${CMAKE_SOURCE_DIR}/build_esp32)
set(ESP32_PORT "/dev/tty.usbmodem1101" CACHE STRING "ESP32 serial port")

# ----------------------------
# Źródła wspólne
set(COMMON_SOURCES
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
        src/scenes/tanks/tank_actor.cpp
        src/scenes/tanks/tank_actor.hpp
        src/scenes/tanks/obstacle_actor.cpp
        src/scenes/tanks/obstacle_actor.hpp
        src/scenes/tanks/obstacle_type.hpp
        src/scenes/tanks/bullet_actor.cpp
        src/scenes/tanks/bullet_actor.hpp
        src/input/table_input_provider.cpp
        src/input/table_input_provider.hpp
)

# ----------------------------
# Desktop (SFML)
if(BUILD_DESKTOP)
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} main_desktop.cpp)
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TARGET_DESKTOP)
endif()

# ----------------------------
# ESP32 (ESP-IDF)
if(BUILD_ESP32)
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    idf_build_process(esp32s3)
    add_executable(${PROJECT_NAME}_esp32 ${COMMON_SOURCES} main_esp32.cpp)
    target_compile_definitions(${PROJECT_NAME}_esp32 PRIVATE TARGET_ESP32)
    idf_build_executable(${PROJECT_NAME}_esp32)
endif()

# ----------------------------
# Custom targety wołane z root
add_custom_target(desktop
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DESKTOP_BLD}
        COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${DESKTOP_BLD}
        -DBUILD_DESKTOP=ON -DBUILD_ESP32=OFF -G "Unix Makefiles"
        COMMAND ${CMAKE_COMMAND} --build ${DESKTOP_BLD} --parallel
        USES_TERMINAL
)

add_custom_target(desktop_run
        COMMAND ${CMAKE_COMMAND} -E chdir ${DESKTOP_BLD} ./gaming_coffee_table_cpp
        DEPENDS desktop
        USES_TERMINAL
)

add_custom_target(esp32
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ESP32_BLD}
        COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${ESP32_BLD}
        -DBUILD_DESKTOP=OFF -DBUILD_ESP32=ON -G "Unix Makefiles"
        COMMAND ${CMAKE_COMMAND} --build ${ESP32_BLD} --parallel
        USES_TERMINAL
)

add_custom_target(esp32_run
        COMMAND idf.py -B ${ESP32_BLD} -p ${ESP32_PORT} flash monitor
        DEPENDS esp32
        USES_TERMINAL
)

add_custom_target(clean_all
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${DESKTOP_BLD} ${ESP32_BLD}
        USES_TERMINAL
)
