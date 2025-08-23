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
    v2 _anchor_offset;
    v2 original_size;
    float _rotation = 0.0f;
    int render_importance = 0;

protected:
    actor(const std::string &name, const v2 &center, const v2 &size);

public:
    virtual ~actor() = default;

    v2 get_center() const;
    void set_center(const v2 &new_center);
    v2 get_anchor_offset() const;
    void set_anchor_offset(const v2 &new_anchor_offset);
    v2 size() const;
    float get_rotation() const;
    std::string name() const;
    void rename(const std::string &new_name);
    void resize(const v2 &new_size);
    void rotate(float degrees);
    actor *rotate_around(const v2 &around, float degrees);
    void set_rotation(float degrees);
    actor *move_to(const v2 &to);
    actor *move_by(const v2 &by);
    void kill();
    void set_render_importance(int order);
    int get_render_importance();

    virtual void redraw() = 0;
    virtual void update(float delta_time) = 0;
    virtual matrix render() = 0;
    virtual void fixed_update(float fixed_delta_time) = 0;

private:
    void set_size(const v2 &new_size);
};
