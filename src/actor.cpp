#include "actor.hpp"
#include "engine.hpp"

actor::actor(const std::string &name, const v2 &center, const v2 &size)
    : _name(name), _center(center.copy()), _size(size), original_size(size), _anchor_offset(v2::zero()) {
}

v2 actor::get_center() const {
    return _center;
}

void actor::set_center(const v2 &new_center) {
    _center = new_center;
}

v2 actor::get_anchor_offset() const {
    return _anchor_offset;
}

void actor::set_anchor_offset(const v2 &new_anchor_offset) {
    _anchor_offset = new_anchor_offset;
}

v2 actor::size() const {
    return _size;
}

float actor::get_rotation() const {
    return _rotation;
}

std::string actor::name() const {
    return _name;
}

void actor::rename(const std::string &new_name) {
    _name = new_name;
}

void actor::resize(const v2 &new_size) {
    set_size(new_size);
    redraw();
}

void actor::rotate(float degrees) {
    set_rotation(degrees + _rotation);
}

actor *actor::rotate_around(const v2 &around, float degrees) {
    set_rotation(degrees + _rotation);
    _center = _center.rotate_around(around, degrees);

    return this;
}

void actor::set_rotation(float degrees) {
    float radians = degrees * static_cast<float>(M_PI) / 180.0f;
    float cos_r = std::cos(radians);
    float sin_r = std::sin(radians);

    float new_x = original_size.x * cos_r - original_size.y * sin_r;
    float new_y = original_size.x * sin_r + original_size.y * cos_r;

    set_size(v2(std::round(std::abs(new_x)), std::round(std::abs(new_y))));

    _rotation = std::fmod(degrees, 360.0f);
    redraw();
}

actor *actor::move_to(const v2 &to) {
    _center = to;
    redraw();

    return this;
}

actor *actor::move_by(const v2 &by) {
    _center = _center.add(by);
    redraw();

    return this;
}

void actor::kill() {
    engine::unregister_actor(shared_from_this());
}

void actor::set_size(const v2 &new_size) {
    _size = v2(std::abs(new_size.x), std::abs(new_size.y));
}
