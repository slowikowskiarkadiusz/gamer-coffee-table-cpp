Desktop:
clear && cmake -B build -DBUILD_DESKTOP=ON -DBUILD_ESP32=OFF . && cmake --build build && ./build/gaming_coffee_table_cpp

ESP32:
clear && cmake -B build -DBUILD_DESKTOP=OFF -DBUILD_ESP32=ON . && cmake --build build