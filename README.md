Desktop:

```
cd desktop
clear && cmake -S . -B build-desktop -DBUILD_DESKTOP=ON -DBUILD_ESP32=OFF && make && ./gaming_coffee_table_cpp_desktop
```

```
rm compile_commands.json && ln -sf build-desktop/compile_commands.json compile_commands.json
```

ESP32:

```
cd esp32
source ~/esp/esp-idf/export.sh
idf.py -B build-esp32

ls /dev/tty.*
clear && idf.py -B build-esp32 -p /dev/tty.usbmodem1101 flash monitor
```

```
rm compile_commands.json && ln -sf esp32/build-esp32/compile_commands.json compile_commands.json
```
