//
// Created by Arkadiusz on 28/07/2025.
//

#include "rectangle-actor.hpp"
#include "../v2.hpp"
#include "../color.hpp"
#include "../engine.hpp"

rectangle_actor rectangle_actor::instantiate(const v2 &center, const v2 &size, const color &col = color::white()) {
    auto rect_actor = rectangle_actor(center, size, col);
    engine::instance().register_actor(std::make_shared<rectangle_actor>(rect_actor));
    return rect_actor;
}
