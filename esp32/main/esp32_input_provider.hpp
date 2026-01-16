#pragma once

#include "../../src/input/input_provider.hpp"
#ifdef PLATFORM_ESP32
#include "../components/hub75/src/ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#endif
#include "driver/gpio.h"
#include "input/key_state.hpp"
#include <unordered_set>

class esp32_input_provider : public input_provider {
  std::vector<gpio_num_t> pins{
      GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19, GPIO_NUM_20, GPIO_NUM_21,
      GPIO_NUM_35, GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_38, GPIO_NUM_39,
      GPIO_NUM_40, GPIO_NUM_41, GPIO_NUM_42, GPIO_NUM_47, GPIO_NUM_48};
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
  static inline bool contains_pin(std::span<const gpio_num_t> lst,
                                  gpio_num_t p) {
    return std::find(lst.begin(), lst.end(), p) != lst.end();
  }

  bool isKey(std::optional<key> k, key_state ks) const {
    // std::cout << " keysDown " << keysDown.size() << std::endl;
    // std::cout << " keysUp " << keysUp.size() << std::endl;
    // std::cout << " keysPress " << keysPress.size() << std::endl;

    if(!(k == key::P1_DOWN ||k == key::P1_UP ||k == key::P1_LEFT ||k == key::P1_RIGHT))
        return false;

    if (!k.has_value()) {
        switch (ks) {
            case key_state::Down: return !keysDown.empty();
            case key_state::Up: return !keysUp.empty();
            case key_state::Press: return !keysPress.empty();
        }
        return false;
    }

    auto pinsForKey = map_key(*k);
    auto any_match = [&](const auto &S) {
      return std::any_of(S.begin(), S.end(), [&](gpio_num_t pin) {
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
    std::array<gpio_num_t, Max> buf{};
    size_t len = 0;
    bool inited = false;
  };

  template <size_t Max>
  static inline std::span<const gpio_num_t>
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

  static inline std::span<const gpio_num_t> map_key(key k) {
    switch (k) {
    case key::START: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_17};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P1_DOWN: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_18};
      static constexpr gpio_num_t A[]{GPIO_NUM_39};
      return A;
    }
    case key::P1_UP: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_19};
      static constexpr gpio_num_t A[]{GPIO_NUM_36};
      return A;
    }
    case key::P1_LEFT: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_20};
      static constexpr gpio_num_t A[]{GPIO_NUM_35};
      return A;
    }
    case key::P1_RIGHT: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_21};
      static constexpr gpio_num_t A[]{GPIO_NUM_37};
      return A;
    }
    case key::P1_ANY_DIRECTION: {
      static cached_span_t<4> C;
      return ensure_init(
          C, {key::P1_DOWN, key::P1_UP, key::P1_LEFT, key::P1_RIGHT});
    }
    case key::P1_BLUE: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_35};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P1_GREEN: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_36};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P1_ANY: {
      static cached_span_t<6> C;
      return ensure_init(C,
                         {key::P1_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN});
    }

    case key::P2_DOWN: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_37};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P2_UP: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_38};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P2_LEFT: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_39};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P2_RIGHT: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_40};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P2_ANY_DIRECTION: {
      static cached_span_t<4> C;
      return ensure_init(
          C, {key::P2_DOWN, key::P2_UP, key::P2_LEFT, key::P2_RIGHT});
    }
    case key::P2_BLUE: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_41};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
      return A;
    }
    case key::P2_GREEN: {
      // static constexpr gpio_num_t A[]{GPIO_NUM_42};
      static constexpr gpio_num_t A[]{GPIO_NUM_1};
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
