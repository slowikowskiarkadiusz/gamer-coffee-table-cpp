//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once
#include "Key.hpp"

class InputProvider {
public:
    virtual ~InputProvider() = default;
    virtual void update(float delta_time) = 0;
    virtual void lateUpdate(float delta_time) = 0;
    virtual bool isKeyDown(Key key) const = 0;
    virtual bool isAnyKeyDown() const = 0;
    virtual bool isKeyUp(Key key) const = 0;
    virtual bool isAnyKeyUp() const = 0;
    virtual bool isKeyPress(Key key) const = 0;
    virtual bool isAnyKeyPress() const = 0;
    virtual void clear() = 0;
};
