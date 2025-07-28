#pragma once

#include "actor.hpp"

class scene {
public:
    std::vector<std::shared_ptr<actor> > actors;

    explicit scene() {
    }

    virtual ~scene() = default;

    virtual void init() = 0;
    virtual void update(float delta_time) = 0;
    virtual void fixed_update(float delta_time) = 0;
};
