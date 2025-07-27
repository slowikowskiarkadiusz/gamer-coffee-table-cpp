//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once
#include "key.hpp"

class input_provider {
public:
    virtual ~input_provider() = default;
    virtual void update(float delta_time) = 0;
    virtual void lateUpdate(float delta_time) = 0;
    virtual bool isKeyDown(key key) const = 0;
    virtual bool isAnyKeyDown() const = 0;
    virtual bool isKeyUp(key key) const = 0;
    virtual bool isAnyKeyUp() const = 0;
    virtual bool isKeyPress(key key) const = 0;
    virtual bool isAnyKeyPress() const = 0;
    virtual void clear() = 0;
};
