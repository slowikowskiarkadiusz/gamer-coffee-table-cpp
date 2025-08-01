#pragma once

#include <memory>

#include "input_provider.hpp"
#include "key.hpp"

class input {
    static input *instance_ptr;
    std::shared_ptr<input_provider> inputProvider;

    static input &instance() {
        return *instance_ptr;
    }

public:
    explicit input(std::shared_ptr<input_provider> inputProvider) {
        this->inputProvider = std::move(inputProvider);
        instance_ptr = this;
    }

    void update(float deltaTime) {
        inputProvider->update(deltaTime);
    }

    void late_update(float deltaTime) {
        inputProvider->lateUpdate(deltaTime);
    }

    static bool is_key_down(key key) {
        return instance().inputProvider->is_key_down(key);
    }

    static bool is_any_key_down() {
        return instance().inputProvider->is_any_key_down();
    }

    static bool is_key_up(key key) {
        return instance().inputProvider->is_key_up(key);
    }

    static bool is_any_key_up() {
        return instance().inputProvider->is_any_key_up();
    }

    static bool is_key_press(key key) {
        return instance().inputProvider->is_key_press(key);
    }

    static bool is_any_key_press() {
        return instance().inputProvider->is_any_key_press();
    }

    void clear() {
        inputProvider->clear();
    }
};
