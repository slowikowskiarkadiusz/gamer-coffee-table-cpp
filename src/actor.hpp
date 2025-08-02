#pragma once

#include "v2.hpp"
#include "matrix.hpp"
#include <string>
#include <memory>

class engine;

class actor : public std::enable_shared_from_this<actor> {
protected:
    std::string _name = "actor";
    v2 _center;
    v2 _size;
    v2 original_size;
    float _rotation = 0.0f;

protected:
    actor(const std::string &name, const v2 &center, const v2 &size);

public:
    virtual ~actor() = default;

    v2 get_center() const;
    void set_center(const v2 &new_center);
    v2 size() const;
    float rotation() const;
    std::string name() const;
    void rename(const std::string &new_name);
    void resize(const v2 &new_size);
    void rotate(float degrees);
    actor *rotate_around(const v2 &around, float degrees);
    void set_rotation(float degrees);
    actor *move_to(const v2 &to);
    actor *move_by(const v2 &by);
    void kill();

    virtual void redraw() = 0;
    virtual void update(float delta_time) = 0;
    virtual matrix render() = 0;
    virtual void fixed_update(float fixed_delta_time) = 0;

private:
    void set_size(const v2 &new_size);
};
