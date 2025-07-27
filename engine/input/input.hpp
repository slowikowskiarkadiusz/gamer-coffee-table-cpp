//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include <memory>

#include "input_provider.hpp"
#include "key.hpp"

#pragma once

class input {
private:
    std::unique_ptr<input_provider> inputProvider;

public:
    explicit input(std::unique_ptr<input_provider> inputProvider) {
        this->inputProvider = std::move(inputProvider);
    }

    void update(float deltaTime) {
        inputProvider->update(deltaTime);
    }

    void lateUpdate(float deltaTime) {
        inputProvider->lateUpdate(deltaTime);
    }

    bool isKeyDown(key key) const {
        return inputProvider->isKeyDown(key);
    }

    bool isAnyKeyDown() const {
        return inputProvider->isAnyKeyDown();
    }

    bool isKeyUp(key key) const {
        return inputProvider->isKeyUp(key);
    }

    bool isAnyKeyUp() const {
        return inputProvider->isAnyKeyUp();
    }

    bool isKeyPress(key key) const {
        return inputProvider->isKeyPress(key);
    }

    bool isAnyKeyPress() const {
        return inputProvider->isAnyKeyPress();
    }

    void clear() {
        inputProvider->clear();
    }
};
