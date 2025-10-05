#ifndef TABLE_INPUT_PROVIDER_H
#define TABLE_INPUT_PROVIDER_H
#include <unordered_set>
#include "../../src/input/input_provider.hpp"
#include "esp_log.h"
#include "driver/gpio.h"
#include "../components/hub75/src/ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "input/key_state.hpp"

class esp32_input_provider : public input_provider {
    std::vector<gpio_num_t> pins{GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19, GPIO_NUM_20, GPIO_NUM_21, GPIO_NUM_35, GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_38, GPIO_NUM_39, GPIO_NUM_40, GPIO_NUM_41, GPIO_NUM_42, GPIO_NUM_47, GPIO_NUM_48};
    std::vector<int> last_level;
    std::unordered_set<gpio_num_t> keysDown;
    std::unordered_set<gpio_num_t> keysUp;
    std::unordered_set<gpio_num_t> keysPress;

public:
    esp32_input_provider() {
        uint64_t mask = 0;
        for (int i = 0; i < pins.size(); i++) {
            mask |= (1ULL << pins[i]);
            last_level.push_back(1);
        }

        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = mask;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        ESP_ERROR_CHECK(gpio_config(&io_conf));
    }

    void update(float delta_time) override {
        for (int i = 0; i < pins.size(); i++) {
            auto pin = pins[i];
            auto level = gpio_get_level(pin);

            if (level != last_level[i]) {
                if (level == 0 && !keysPress.contains(pin)) {
                    if (pin == GPIO_NUM_17)
                        ESP_LOGI("APP", "GPIO_NUM_17");
                    keysDown.insert(pin);
                    keysPress.insert(pin);
                }

                if (level == 1) {
                    keysUp.insert(pin);
                    keysPress.erase(pin);
                }
            }

            last_level[i] = level;
        }
    }

    void late_update(float delta_time) override {
        this->keysDown.clear();
        this->keysUp.clear();
    }

    bool is_key_down(key key) const override {
        return isKey(key, key_state::Down);
    }

    bool is_any_key_down() const override {
        return isKey(std::nullopt, key_state::Down);
    }

    bool is_key_up(key key) const override {
        return isKey(key, key_state::Up);
    }

    bool is_any_key_up() const override {
        return isKey(std::nullopt, key_state::Up);
    }

    bool is_key_press(key key) const override {
        return isKey(key, key_state::Press);
    }

    bool is_any_key_press() const override {
        return isKey(std::nullopt, key_state::Press);
    }

    void clear() override {
        this->keysDown.clear();
        this->keysUp.clear();
        this->keysPress.clear();
    }

private:
    static inline bool contains_pin(std::span<const gpio_num_t> lst, gpio_num_t p) {
        return std::find(lst.begin(), lst.end(), p) != lst.end();
    }

    bool isKey(std::optional<key> k, key_state ks) const {
        if (!k.has_value()) {
            // „any” bez konkretnego klawisza
            switch (ks) {
                case key_state::Down: return !keysDown.empty();
                case key_state::Up: return !keysUp.empty();
                case key_state::Press: return !keysPress.empty();
            }
            return false;
        }

        auto pinsForKey = map_key(*k);
        auto any_match = [&](const auto &S) {
            return std::any_of(S.begin(), S.end(),
                               [&](gpio_num_t pin) { return contains_pin(pinsForKey, pin); });
        };

        switch (ks) {
            case key_state::Down: return any_match(keysDown);
            case key_state::Up: return any_match(keysUp);
            case key_state::Press: return any_match(keysPress);
        }
        return false;
    }

    std::unordered_set<gpio_num_t> map_key(key key) const {
        std::unordered_set<gpio_num_t> a;
        std::unordered_set<gpio_num_t> b;

        switch (key) {
            case key::START:
                return {GPIO_NUM_17};
            case key::P1_DOWN:
                return {GPIO_NUM_18};
            case key::P1_UP:
                return {GPIO_NUM_19};
            case key::P1_LEFT:
                return {GPIO_NUM_20};
            case key::P1_RIGHT:
                return {GPIO_NUM_21};
            case key::P1_ANY_DIRECTION:
                for (const auto k: {key::P1_DOWN, key::P1_UP, key::P1_LEFT, key::P1_RIGHT}) {
                    auto b = map_key(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P1_BLUE:
                return {GPIO_NUM_35};
            case key::P1_GREEN:
                return {GPIO_NUM_36};
            case key::P1_ANY:
                for (const auto k: {key::P1_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN}) {
                    auto b = map_key(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P2_DOWN:
                return {GPIO_NUM_37};
            case key::P2_UP:
                return {GPIO_NUM_38};
            case key::P2_LEFT:
                return {GPIO_NUM_39};
            case key::P2_RIGHT:
                return {GPIO_NUM_40};
            case key::P2_ANY_DIRECTION:
                for (const auto k: {key::P2_DOWN, key::P2_UP, key::P2_LEFT, key::P2_RIGHT}) {
                    auto b = map_key(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P2_BLUE:
                return {GPIO_NUM_41};
            case key::P2_GREEN:
                return {GPIO_NUM_42};
            case key::P2_ANY:
                for (const auto k: {key::P2_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN}) {
                    auto b = map_key(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            default: return {};
        }
    }
};

#endif //TABLE_INPUT_PROVIDER_H
