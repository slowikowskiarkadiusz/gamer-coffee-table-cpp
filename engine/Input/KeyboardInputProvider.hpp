//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include <iostream>
#include <unordered_set>

#include "InputProvider.hpp"
#include "KeyState.hpp"

class KeyboardInputProvider : public InputProvider {
private:
    sf::RenderWindow *window;
    std::unordered_set<sf::Keyboard::Key> keysDown;
    std::unordered_set<sf::Keyboard::Key> keysUp;
    std::unordered_set<sf::Keyboard::Key> keysPress;

public:
    explicit KeyboardInputProvider(sf::RenderWindow *window) {
        this->window = window;
    }

    ~KeyboardInputProvider() override = default;

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

    bool isKeyDown(Key key) const override {
        return isKey(key, KeyState::Down);
    }

    bool isAnyKeyDown() const override {
        return isKey(std::nullopt, KeyState::Down);
    }

    bool isKeyUp(Key key) const override {
        return isKey(key, KeyState::Up);
    }

    bool isAnyKeyUp() const override {
        return isKey(std::nullopt, KeyState::Up);
    }

    bool isKeyPress(Key key) const override {
        return isKey(key, KeyState::Press);
    }

    bool isAnyKeyPress() const override {
        return isKey(std::nullopt, KeyState::Press);
    }

    void clear() override {
        this->keysDown.clear();
        this->keysUp.clear();
        this->keysPress.clear();
    }

private:
    bool isKey(std::optional<Key> key, KeyState key_state) const {
        std::unordered_set<sf::Keyboard::Key> mappedKey = key.has_value()
                                                              ? mapKey(key.value())
                                                              : std::unordered_set<sf::Keyboard::Key>();

        switch (key_state) {
            case KeyState::Down:
                return key.has_value()
                           ? std::any_of(keysDown.begin(), keysDown.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysDown.empty();
            case KeyState::Up:
                return key.has_value()
                           ? std::any_of(keysUp.begin(), keysUp.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysUp.empty();
            case KeyState::Press:
                return key.has_value()
                           ? std::any_of(keysPress.begin(), keysPress.end(), [mappedKey](sf::Keyboard::Key x) {
                               return mappedKey.contains(x);
                           })
                           : !keysPress.empty();

            default: return false;
        }
    }

    std::unordered_set<sf::Keyboard::Key> mapKey(Key key) const {
        std::unordered_set<sf::Keyboard::Key> a;
        std::unordered_set<sf::Keyboard::Key> b;

        switch (key) {
            case Key::P1_L_L:
                return {sf::Keyboard::Z};
            case Key::P1_L_BLUE:
                return {sf::Keyboard::A};
            case Key::P1_L:
                a = mapKey(Key::P1_L_L);
                b = mapKey(Key::P1_L_BLUE);
                a.insert(b.begin(), b.end());
                return a;
            case Key::P1_R_R:
                return {sf::Keyboard::X};
            case Key::P1_R_GREEN:
                return {sf::Keyboard::D};
            case Key::P1_R:
                a = mapKey(Key::P1_R_R);
                b = mapKey(Key::P1_R_GREEN);
                a.insert(b.begin(), b.end());
                return a;
            case Key::P2_L_L:
                return {sf::Keyboard::N};
            case Key::P2_L_BLUE:
                return {sf::Keyboard::H};
            case Key::P2_L:
                a = mapKey(Key::P2_L_L);
                b = mapKey(Key::P2_L_BLUE);
                a.insert(b.begin(), b.end());
                return a;
            case Key::P2_R_R:
                return {sf::Keyboard::M};
            case Key::P2_R_GREEN:
                return {sf::Keyboard::K};
            case Key::P2_R:
                a = mapKey(Key::P2_R_R);
                b = mapKey(Key::P2_R_GREEN);
                a.insert(b.begin(), b.end());
                return a;
            case Key::START:
                return {sf::Keyboard::Y};
            default: return {};
        }
    }
};
