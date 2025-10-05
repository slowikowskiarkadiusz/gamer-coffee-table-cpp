cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE SRC_FILES
        "../../src/*.cpp"
        "../../src/*.hpp"
        "../../src/*.h"
)

idf_component_register(
        SRCS
        "main_esp32.cpp"
        "esp32_input_provider.hpp"
        ${SRC_FILES}
        INCLUDE_DIRS
        "../../src"
        REQUIRES
        hub75
        log
        pthread
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++20 -Wno-error=reorder -Wno-error=changes-meaning)