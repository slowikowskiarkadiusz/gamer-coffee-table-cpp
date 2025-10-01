Desktop:
```
clear && cmake -S . -B . -G "Unix Makefiles" && make desktop_run && ./build/gaming_coffee_table_cpp
```

ESP32:
```
cd esp32
source ~/esp/esp-idf/export.sh
idf.py -B build_esp32

ls /dev/tty.*
idf.py -B build_esp32 -p /dev/tty.usbmodem11101 flash monitor
```