#pragma once

#include <unordered_set>

#include "input_provider.hpp"
#include "key_state.hpp"

class keyboard_input_provider : public input_provider {
private:
    std::unordered_set<SDL_Keycode> keysDown;
    std::unordered_set<SDL_Keycode> keysUp;
    std::unordered_set<SDL_Keycode> keysPress;

public:
    std::queue<SDL_Event> sdlEventQueue = std::queue<SDL_Event>();

    explicit keyboard_input_provider() {
    }

    ~keyboard_input_provider() override = default;

    void update(float delta_time) override {
        while (!sdlEventQueue.empty()) {
            auto event = sdlEventQueue.front();
            sdlEventQueue.pop();

            if (event.type == SDL_KEYDOWN && !keysPress.contains(event.key.keysym.sym)) {
                keysDown.insert(event.key.keysym.sym);
                keysPress.insert(event.key.keysym.sym);
            }

            if (event.type == SDL_KEYUP) {
                keysUp.insert(event.key.keysym.sym);
                keysPress.erase(event.key.keysym.sym);
            }
        }
    }

    void lateUpdate(float delta_time) override {
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
    bool isKey(std::optional<key> key, key_state key_state) const {
        std::unordered_set<SDL_Keycode> mappedKey = key.has_value()
                                                        ? mapKey(key.value())
                                                        : std::unordered_set<SDL_Keycode>();

        switch (key_state) {
            case key_state::Down:
                return key.has_value()
                           ? std::any_of(keysDown.begin(), keysDown.end(), [mappedKey](SDL_Keycode x) {
                               return mappedKey.contains(x);
                           })
                           : !keysDown.empty();
            case key_state::Up:
                return key.has_value()
                           ? std::any_of(keysUp.begin(), keysUp.end(), [mappedKey](SDL_Keycode x) {
                               return mappedKey.contains(x);
                           })
                           : !keysUp.empty();
            case key_state::Press:
                return key.has_value()
                           ? std::any_of(keysPress.begin(), keysPress.end(), [mappedKey](SDL_Keycode x) {
                               return mappedKey.contains(x);
                           })
                           : !keysPress.empty();

            default: return false;
        }
    }

    std::unordered_set<SDL_Keycode> mapKey(key key) const {
        std::unordered_set<SDL_Keycode> a;
        // std::unordered_set<SDL_Keycode> b;

        switch (key) {
            case key::START:
                return {SDLK_y};
            case key::P1_DOWN:
                return {SDLK_s};
            case key::P1_UP:
                return {SDLK_w};
            case key::P1_LEFT:
                return {SDLK_a};
            case key::P1_RIGHT:
                return {SDLK_d};
            case key::P1_ANY_DIRECTION:
                for (const auto k: {key::P1_DOWN, key::P1_UP, key::P1_LEFT, key::P1_RIGHT}) {
                    auto b = mapKey(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P1_BLUE:
                return {SDLK_f};
            case key::P1_GREEN:
                return {SDLK_g};
            case key::P1_ANY:
                for (const auto k: {key::P1_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN}) {
                    auto b = mapKey(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P2_DOWN:
                return {SDLK_DOWN};
            case key::P2_UP:
                return {SDLK_UP};
            case key::P2_LEFT:
                return {SDLK_LEFT};
            case key::P2_RIGHT:
                return {SDLK_RIGHT};
            case key::P2_ANY_DIRECTION:
                for (const auto k: {key::P2_DOWN, key::P2_UP, key::P2_LEFT, key::P2_RIGHT}) {
                    auto b = mapKey(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            case key::P2_BLUE:
                return {SDLK_o};
            case key::P2_GREEN:
                return {SDLK_p};
            case key::P2_ANY:
                for (const auto k: {key::P2_ANY_DIRECTION, key::P1_BLUE, key::P1_GREEN}) {
                    auto b = mapKey(k);
                    a.insert(b.begin(), b.end());
                }
                return a;
            default: return {};
        }
    }
};
