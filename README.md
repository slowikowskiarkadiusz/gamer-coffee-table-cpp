Desktop:

```
cd desktop
clear && cmake -S . -B . -G "Unix Makefiles" -DBUILD_DESKTOP=ON -DBUILD_ESP32=OFF && make && ./gaming_coffee_table_cpp_desktop
```

ESP32:

```
cd esp32
source ~/esp/esp-idf/export.sh
idf.py -B build-esp32

ls /dev/tty.*
idf.py -B build-esp32 -p /dev/tty.usbmodem11101 flash monitor
```