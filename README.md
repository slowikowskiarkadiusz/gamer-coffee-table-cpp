Desktop:
```
clear && cmake -S . -B . -G "Unix Makefiles" && make desktop_run && ./build/gaming_coffee_table_cpp
```

ESP32:
```
source ~/esp/esp-idf/export.sh
clear && cmake -S . -B . -G "Unix Makefiles" && make esp32_run
```