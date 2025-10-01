file(GLOB_RECURSE SRC_FILES
        "../../src/*.cpp"
        "../../src/*.hpp"
        "../../src/*.h"
)

idf_component_register(
        SRCS
        "main_esp32.cpp"
        ${SRC_FILES}
        INCLUDE_DIRS
        "../../src"
        REQUIRES
        hub75
        arduino-esp32
        Adafruit-GFX-Library
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++20 -Wno-error=reorder -Wno-error=changes-meaning)