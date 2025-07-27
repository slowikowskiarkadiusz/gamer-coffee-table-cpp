//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include <iostream>
#include <unordered_set>

#include "input_provider.hpp"
#include "key_state.hpp"

class keyboard_input_provider : public input_provider {
private:
    sf::RenderWindow *window;
    std::unordered_set<sf::Keyboard::Key> keysDown;
    std::unordered_set<sf::Keyboard::Key> keysUp;
    std::unordered_set<sf::Keyboard::Key> keysPress;

public:
    explicit keyboard_input_provider(sf::RenderWindow *window) {
        this->window = window;
    }

    ~keyboard_input_provider() override = default;

    void update(float delta_time) override {
        sf::Event event{};
        if (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed) {
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

    bool isKeyDown(key key) const override {
        return isKey(key, key_state::Down);
    }

    bool isAnyKeyDown() const override {
        return isKey(std::nullopt, key_state::Down);
    }

    bool isKeyUp(key key) const override {
        return isKey(key, key_state::Up);
    }

    bool isAnyKeyUp() const override {
        return isKey(std::nullopt, key_state::Up);
    }

    bool isKeyPress(key key) const override {
        return isKey(key, key_state::Press);
    }

    bool isAnyKeyPress() const override {
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
