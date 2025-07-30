#include "rectangle_actor.hpp"
#include "../v2.hpp"
#include "../color.hpp"
#include "../engine.hpp"

std::shared_ptr<rectangle_actor> rectangle_actor::instantiate(const v2 &center, const v2 &size, const color &col) {
    auto rect_actor = std::make_shared<rectangle_actor>(center, size, col);
    engine::instance().register_actor(rect_actor);
    return rect_actor;
}

rectangle_actor::~rectangle_actor() {
}
