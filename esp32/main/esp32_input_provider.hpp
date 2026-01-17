#pragma once

#include "../../src/input/input_provider.hpp"
#ifdef PLATFORM_ESP32
#include "../components/hub75/src/ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#endif
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "input/key_state.hpp"
#include <unordered_set>

enum io_pin {
  gpio_1 = 1,
  gpio_17 = 17,
  gpio_18 = 18,
  gpio_19 = 19,
  gpio_20 = 20,
  gpio_21 = 21,
  gpio_35 = 35,
  gpio_36 = 36,
  gpio_37 = 37,
  gpio_38 = 38,
  gpio_39 = 39,
  gpio_40 = 40,
  gpio_41 = 41,
  gpio_42 = 42,
  gpio_47 = 47,
  gpio_48 = 48,
  exp_p0 = 90,
  exp_p1 = 91,
  exp_p2 = 92,
  exp_p3 = 93,
  exp_p4 = 94,
  exp_p5 = 95,
  exp_p6 = 96,
  exp_p7 = 97,
};

class esp32_input_provider : public input_provider {
  std::vector<io_pin> pins{
      gpio_17, gpio_18, gpio_19, gpio_20, gpio_21, gpio_35, gpio_36,
      gpio_37, gpio_38, gpio_39, gpio_40, gpio_47, gpio_48, exp_p0,
      exp_p1,  exp_p2,  exp_p3,  exp_p4,  exp_p5,  exp_p6,  exp_p7,
  };
  std::vector<int> last_level;
  std::unordered_set<io_pin> keysDown;
  std::unordered_set<io_pin> keysUp;
  std::unordered_set<io_pin> keysPress;

public:
  esp32_input_provider() {
    uint64_t mask = 0;
    for (int i = 0; i < pins.size(); i++) {
      if (!is_expander_pin(pins[i])) {
        mask |= (1ULL << pins[i]);
      }
      last_level.push_back(1);
    }

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = mask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    i2c_config_t conf{};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = GPIO_NUM_41;
    conf.scl_io_num = GPIO_NUM_42;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    uint8_t init = 0xFF;
    // uint8_t init = 0x20;
    // uint8_t init = 0x38;
    i2c_master_write_to_device(I2C_NUM_0, 0x20, &init, 1, pdMS_TO_TICKS(10));
  }

  uint8_t read_expander() {
    uint8_t data = 0xFF;
    i2c_master_read_from_device(I2C_NUM_0, 0x20, &data, 1, pdMS_TO_TICKS(10));
    return data;
  }

  bool exp_get_level(uint8_t expander_data, io_pin pin) {
    return (expander_data & (1 << (pin - io_pin::exp_p0)));
  }

  void update(float delta_time) override {
    auto expander_data = read_expander();
    for (int i = 0; i < pins.size(); i++) {
      auto pin = pins[i];

      auto level = is_expander_pin(pin)
                       ? exp_get_level(expander_data, pin)
                       : gpio_get_level(static_cast<gpio_num_t>(pin));

      if (level != last_level[i]) {
        if (level == 0 && !keysPress.contains(pin)) {
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

  bool is_key_up(key key) const override { return isKey(key, key_state::Up); }

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
  static inline bool contains_pin(std::span<const io_pin> lst, io_pin p) {
    return std::find(lst.begin(), lst.end(), p) != lst.end();
  }

  bool is_expander_pin(io_pin pin) { return pin >= 90; }

  bool isKey(std::optional<key> k, key_state ks) const {
    if (!(k == key::P1_DOWN || k == key::P1_UP || k == key::P1_LEFT ||
          k == key::P1_RIGHT || k == key::P1_BLUE || k == key::P1_GREEN))
      return false;

    if (!k.has_value()) {
      switch (ks) {
      case key_state::Down:
        return !keysDown.empty();
      case key_state::Up:
        return !keysUp.empty();
      case key_state::Press:
        return !keysPress.empty();
      }
      return false;
    }

    auto pinsForKey = map_key(*k);
    auto any_match = [&](const auto &S) {
      return std::any_of(S.begin(), S.end(), [&](io_pin pin) {
        return contains_pin(pinsForKey, pin);
      });
    };

    switch (ks) {
    case key_state::Down:
      return any_match(keysDown);
    case key_state::Up:
      return any_match(keysUp);
    case key_state::Press:
      return any_match(keysPress);
    }
    return false;
  }

  template <size_t Max> struct cached_span_t {
    std::array<io_pin, Max> buf{};
    size_t len = 0;
    bool inited = false;
  };

  template <size_t Max>
  static inline std::span<const io_pin>
  ensure_init(cached_span_t<Max> &c, std::initializer_list<key> parts) {
    if (!c.inited) {
      c.len = 0;
      for (key k : parts) {
        auto s = map_key(k);
        for (auto p : s) {
          bool exists = false;
          for (size_t i = 0; i < c.len; ++i)
            if (c.buf[i] == p) {
              exists = true;
              break;
            }
          if (!exists)
            c.buf[c.len++] = p;
        }
      }
      c.inited = true;
    }
    return {c.buf.data(), c.len};
  }

  static inline std::span<const io_pin> map_key(key k) {
    switch (k) {
    case key::START: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P1_DOWN: {
      static constexpr io_pin A[]{gpio_39};
      return A;
    }
    case key::P1_UP: {
      static constexpr io_pin A[]{gpio_36};
      return A;
    }
    case key::P1_LEFT: {
      static constexpr io_pin A[]{gpio_35};
      return A;
    }
    case key::P1_RIGHT: {
      static constexpr io_pin A[]{gpio_37};
      return A;
    }
    case key::P1_ANY_DIRECTION: {
      static cached_span_t<4> C;
      return ensure_init(
          C, {key::P1_DOWN, key::P1_UP, key::P1_LEFT, key::P1_RIGHT});
    }
    case key::P1_BLUE: {
      static constexpr io_pin A[]{gpio_47};
      return A;
    }
    case key::P1_GREEN: {
      static constexpr io_pin A[]{gpio_48};
      return A;
    }
    case key::P1_ANY: {
      static cached_span_t<6> C;
      return ensure_init(C,
                         {key::P1_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN});
    }

    case key::P2_DOWN: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_UP: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_LEFT: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_RIGHT: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_ANY_DIRECTION: {
      static cached_span_t<4> C;
      return ensure_init(
          C, {key::P2_DOWN, key::P2_UP, key::P2_LEFT, key::P2_RIGHT});
    }
    case key::P2_BLUE: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_GREEN: {
      static constexpr io_pin A[]{gpio_1};
      return A;
    }
    case key::P2_ANY: {
      static cached_span_t<6> C;
      return ensure_init(C,
                         {key::P2_ANY_DIRECTION, key::P2_BLUE, key::P2_GREEN});
    }

    default:
      return {};
    }
  }
};
