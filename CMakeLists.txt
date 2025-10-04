cmake_minimum_required(VERSION 3.16)

project(gaming_coffee_table_cpp LANGUAGES C CXX)

option(BUILD_DESKTOP "Build desktop target" ON)
option(BUILD_ESP32 "Build ESP-IDF target" OFF)


if (BUILD_ESP32)
    if (NOT DEFINED ENV{IDF_PATH})
        message(FATAL_ERROR "IDF_PATH nie jest ustawiony. Uruchom w terminalu: 'source ~/esp/esp-idf/export.sh' albo ustaw IDF_PATH w profilu CMake (Environment).")
    endif ()

    message(STATUS "==> ESP32: włączam podprojekt esp32/")
    add_subdirectory(esp32)
else ()
    message(STATUS "==> Desktop: włączam podprojekt desktop/")
    add_subdirectory(desktop)
endif ()
