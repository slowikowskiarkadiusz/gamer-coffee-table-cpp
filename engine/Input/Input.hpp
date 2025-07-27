//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include <memory>

#include "InputProvider.hpp"
#include "Key.hpp"

#pragma once

class Input {
private:
    std::unique_ptr<InputProvider> inputProvider;

public:
    explicit Input(std::unique_ptr<InputProvider> inputProvider) {
        this->inputProvider = std::move(inputProvider);
    }

    void update(float deltaTime) {
        inputProvider->update(deltaTime);
    }

    void lateUpdate(float deltaTime) {
        inputProvider->lateUpdate(deltaTime);
    }

    bool isKeyDown(Key key) const {
        return inputProvider->isKeyDown(key);
    }

    bool isAnyKeyDown() const {
        return inputProvider->isAnyKeyDown();
    }

    bool isKeyUp(Key key) const {
        return inputProvider->isKeyUp(key);
    }

    bool isAnyKeyUp() const {
        return inputProvider->isAnyKeyUp();
    }

    bool isKeyPress(Key key) const {
        return inputProvider->isKeyPress(key);
    }

    bool isAnyKeyPress() const {
        return inputProvider->isAnyKeyPress();
    }

    void clear() {
        inputProvider->clear();
    }
};
