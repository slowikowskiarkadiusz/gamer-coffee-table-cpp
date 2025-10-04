//
// Created by Arkadiusz on 30/09/2025.
//

#include "table_input_provider.hpp"

table_input_provider::table_input_provider() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE; // brak przerwań
    io_conf.mode = GPIO_MODE_INPUT; // tryb wejścia
    io_conf.pin_bit_mask = (1ULL << GPIO_NUM_18); // wybór GPIO1
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; // np. włącz pull-up
    gpio_config(&io_conf);
    auto lastState = 0;
}

void table_input_provider::update(float delta_time) {
}

void table_input_provider::lateUpdate(float delta_time) {
}

bool table_input_provider::is_key_down(key key) const {
    return false;
}

bool table_input_provider::is_any_key_down() const {
    return false;
}

bool table_input_provider::is_key_up(key key) const {
    return false;
}

bool table_input_provider::is_any_key_up() const {
    return false;
}

bool table_input_provider::is_key_press(key key) const {
    return false;
}

bool table_input_provider::is_any_key_press() const {
    return false;
}

void table_input_provider::clear() {
}
