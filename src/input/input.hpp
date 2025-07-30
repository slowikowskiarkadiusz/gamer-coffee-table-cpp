#pragma once

#include <memory>

#include "input_provider.hpp"
#include "key.hpp"

class input {
    static input *instance_ptr;
    std::shared_ptr<input_provider> inputProvider;

public:
    explicit input(std::shared_ptr<input_provider> inputProvider) {
        this->inputProvider = std::move(inputProvider);
        instance_ptr = this;
    }

    static input &i() {
        return *instance_ptr;
    }

    void update(float deltaTime) {
        inputProvider->update(deltaTime);
    }

    void late_update(float deltaTime) {
        inputProvider->lateUpdate(deltaTime);
    }

    bool is_key_down(key key) const {
        return inputProvider->is_key_down(key);
    }

    bool is_any_key_down() const {
        return inputProvider->is_any_key_down();
    }

    bool is_key_up(key key) const {
        return inputProvider->is_key_up(key);
    }

    bool is_any_key_up() const {
        return inputProvider->is_any_key_up();
    }

    bool is_key_press(key key) const {
        return inputProvider->is_key_press(key);
    }

    bool is_any_key_press() const {
        return inputProvider->is_any_key_press();
    }

    void clear() {
        inputProvider->clear();
    }
};
