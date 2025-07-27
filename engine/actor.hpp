//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include "v2.hpp"
#include "matrix.hpp"
#include <string>
#include <cmath>

class engine;

class actor {
protected:
    std::string _name = "actor";
    v2 _center;
    v2 _size;
    v2 original_size;
    float _rotation = 0.0f;

public:
    actor(const std::string &name, const v2 &center, const v2 &size)
        : _name(name), _center(center.copy()), _size(size), original_size(size) {
        engine::instance().register_actor(this);
    }

    virtual ~actor() = default;

    v2 center() const {
        return _center;
    }

    void set_center(const v2 &new_center) {
        _center = new_center;
    }

    v2 size() const {
        return _size;
    }

    float rotation() const {
        return _rotation;
    }

    std::string name() const {
        return _name;
    }

    void rename(const std::string &new_name) {
        _name = new_name;
    }

    void resize(const v2 &new_size) {
        set_size(new_size);
        redraw();
    }

    void rotate(float degrees) {
        set_rotation(degrees + _rotation);
    }

    void rotate_around(const v2 &around, float degrees) {
        set_rotation(degrees + _rotation);
        _center = _center.rotate_around(around, degrees);
    }

    void set_rotation(float degrees) {
        float radians = degrees * static_cast<float>(M_PI) / 180.0f;
        float cos_r = std::cos(radians);
        float sin_r = std::sin(radians);

        float new_x = original_size.x * cos_r - original_size.y * sin_r;
        float new_y = original_size.x * sin_r + original_size.y * cos_r;

        set_size(v2(std::abs(new_x), std::abs(new_y)));

        _rotation = std::fmod(degrees, 360.0f);
        redraw();
    }

    void move_to(const v2 &to) {
        _center = to;
        redraw();
    }

    void move_by(const v2 &by) {
        _center = _center.add(by);
        redraw();
    }

    void kill() {
        engine::instance().unregister_actor(this);
    }

    virtual void redraw() = 0;
    virtual void update(float delta_time) = 0;
    virtual matrix render() = 0;
    virtual void fixed_update(float fixed_delta_time) = 0;

private:
    void set_size(const v2 &new_size) {
        _size = v2(std::abs(new_size.x), std::abs(new_size.y));
    }
};
