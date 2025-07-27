//
// Created by Arkadiusz on 27/07/2025.
//

#ifndef SCENES_HPP
#define SCENES_HPP
#include "actor.hpp"

template<typename T>
class scene {
public:
    std::vector<std::shared_ptr<actor> > actors;

    explicit scene(T param) {
    }

    virtual ~scene() = default;

    virtual void update(float delta_time) = 0;
    virtual void fixed_update(float delta_time) = 0;
};

#endif //SCENES_HPP
