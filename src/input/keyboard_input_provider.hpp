#pragma once

#include <iostream>
#include <unordered_set>

#include "input_provider.hpp"
#include "key_state.hpp"

class keyboard_input_provider : public input_provider {
private:
    std::unordered_set<sf::Keyboard::Key> keysDown;
    std::unordered_set<sf::Keyboard::Key> keysUp;
    std::unordered_set<sf::Keyboard::Key> keysPress;

public:
    std::queue<sf::Event> sfEventQueue = std::queue<sf::Event>();

    explicit keyboard_input_provider() {
    }

    ~keyboard_input_provider() override = default;

    void update(float delta_time) override {
        while (!sfEventQueue.empty()) {
            auto event = sfEventQueue.front();
            sfEventQueue.pop();

            if (event.type == sf::Event::KeyPressed && !keysPress.contains(event.key.code)) {
                keysDown.insert(event.key.code);
                keysPress.insert(event.key.code);
            }

            if (event.type == sf::Event::KeyReleased) {
                keysUp.insert(event.key.code);
                keysPress.erase(event.key.code);
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
        std::unordered_set<sf::Keyboard::Key> mappedKey = key.has_value()
                                                              ? mapKey(key.value())
                                                              : std::unordered_set<sf::Keyboard::Key>();

        switch (key_state) {
            case key_state::Down:
                return key.has_value()
                           ? std::any_of(keysDown.begin(), keysDown.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysDown.empty();
            case key_state::Up:
                return key.has_value()
                           ? std::any_of(keysUp.begin(), keysUp.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysUp.empty();
            case key_state::Press:
                return key.has_value()
                           ? std::any_of(keysPress.begin(), keysPress.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysPress.empty();

            default: return false;
        }
    }

    std::unordered_set<sf::Keyboard::Key> mapKey(key key) const {
        std::unordered_set<sf::Keyboard::Key> a;
        std::unordered_set<sf::Keyboard::Key> b;

        switch (key) {
            case key::P1_L_L:
                return {sf::Keyboard::Z};
            case key::P1_L_BLUE:
                return {sf::Keyboard::A};
            case key::P1_L:
                a = mapKey(key::P1_L_L);
                b = mapKey(key::P1_L_BLUE);
                a.insert(b.begin(), b.end());
                return a;
            case key::P1_R_R:
                return {sf::Keyboard::X};
            case key::P1_R_GREEN:
                return {sf::Keyboard::D};
            case key::P1_R:
                a = mapKey(key::P1_R_R);
                b = mapKey(key::P1_R_GREEN);
                a.insert(b.begin(), b.end());
                return a;
            case key::P2_L_L:
                return {sf::Keyboard::N};
            case key::P2_L_BLUE:
                return {sf::Keyboard::H};
            case key::P2_L:
                a = mapKey(key::P2_L_L);
                b = mapKey(key::P2_L_BLUE);
                a.insert(b.begin(), b.end());
                return a;
            case key::P2_R_R:
                return {sf::Keyboard::M};
            case key::P2_R_GREEN:
                return {sf::Keyboard::K};
            case key::P2_R:
                a = mapKey(key::P2_R_R);
                b = mapKey(key::P2_R_GREEN);
                a.insert(b.begin(), b.end());
                return a;
            case key::START:
                return {sf::Keyboard::Y};
            default: return {};
        }
    }
};
