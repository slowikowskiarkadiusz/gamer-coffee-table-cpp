#pragma once
#include "key.hpp"

class input_provider {
public:
    virtual ~input_provider() = default;
    virtual void update(float delta_time) = 0;
    virtual void late_update(float delta_time) = 0;
    virtual bool is_key_down(key key) const = 0;
    virtual bool is_any_key_down() const = 0;
    virtual bool is_key_up(key key) const = 0;
    virtual bool is_any_key_up() const = 0;
    virtual bool is_key_press(key key) const = 0;
    virtual bool is_any_key_press() const = 0;
    virtual void clear() = 0;
};
