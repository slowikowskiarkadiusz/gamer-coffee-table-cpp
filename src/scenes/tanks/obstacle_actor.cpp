#include "obstacle_actor.hpp"

void obstacle_actor::redraw() {
    matrix_ = matrix(size().x, size().y);
    auto grass = color(168.0 / 255.0, 230.0 / 255.0, 29.0 / 255.0);
    auto light_brick = color(156.0 / 255.0, 90.0 / 255.0, 60.0 / 255.0);
    auto dark_brick = color(138.0 / 255.0, 63.0 / 255.0, 30.0 / 255.0);
    auto third_brick = color(138.0 / 255.0, 87.0 / 255.0, 65.0 / 255.0);
    auto light_steel = color(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
    auto dark_steel = color(70.0 / 255.0, 70.0 / 255.0, 70.0 / 255.0);
    auto light_water = color(153.0 / 255.0, 217.0 / 255.0, 234.0 / 255.0);
    auto dark_water = color(77.0 / 255.0, 109.0 / 255.0, 243.0 / 255.0);

    switch (obstacle_type_) {
        case obstacle_type::none:
            break;
        case obstacle_type::grass:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_y_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    auto is_x_odd = x % 2 == 1;
                    if (is_y_odd == is_x_odd)
                        matrix_.set_pixel(x, y, grass);
                }
            }
            break;
        case obstacle_type::brick:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    if (is_odd && x == matrix_.width() - 1 || !is_odd && x == 0)
                        matrix_.set_pixel(x, y, third_brick);
                    else
                        matrix_.set_pixel(x, y, is_odd ? light_brick : dark_brick);
                }
            }
            break;
        case obstacle_type::steel:
            for (int y = 0; y < matrix_.height(); ++y) {
                for (int x = 0; x < matrix_.width(); ++x) {
                    if (x == 0 || x == matrix_.width() - 1 || y == 0 || y == matrix_.height() - 1)
                        matrix_.set_pixel(x, y, light_steel);
                    else
                        matrix_.set_pixel(x, y, dark_steel);
                }
            }
            break;
        case obstacle_type::water:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_y_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    auto is_x_odd = x % 2 == 1;
                    if (is_y_odd == is_x_odd)
                        matrix_.set_pixel(x, y, light_water);
                    else
                        matrix_.set_pixel(x, y, dark_water);
                }
            }
            break;
    }
}

void obstacle_actor::update(float delta_time) {
}

matrix obstacle_actor::render() {
    return matrix_;
}

void obstacle_actor::fixed_update(float fixed_delta_time) {
}

obstacle_actor::~obstacle_actor() = default;
