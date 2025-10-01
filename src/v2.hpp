#pragma once

#import <cmath>
#include <format>
#include <string>

constexpr float pi = 3.141592653589793;

class v2 {
public:
    float x;
    float y;

    v2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    static v2 copy(v2 from) {
        return {from.x, from.y};
    }

    static v2 zero() {
        return {0, 0};
    }

    static v2 one() {
        return {1, 1};
    }

    static v2 n_one() {
        return {-1, -1};
    }

    static v2 up() {
        return {0, -1};
    }

    static v2 down() {
        return {0, 1};
    }

    static v2 left() {
        return {-1, 0};
    }

    static v2 right() {
        return {1, 0};
    }

    static v2 left_down() {
        return {-1, 1};
    }

    static v2 left_up() {
        return {-1, -1};
    }

    static v2 right_down() {
        return {1, 1};
    }

    static v2 right_up() {
        return {1, -1};
    }

    static float dot(v2 v1, v2 v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    static float distance(v2 v1, v2 v2) {
        return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
    }

    v2 copy() const {
        return {this->x, this->y};
    }

    v2 add(v2 v) const {
        return {this->x + v.x, this->y + v.y};
    }

    v2 sub(v2 v) const {
        return {this->x - v.x, this->y - v.y};
    }

    v2 mul(float scalar) const {
        return {this->x * scalar, this->y * scalar};
    }

    v2 div(float scalar) const {
        return {this->x / scalar, this->y / scalar};
    }

    v2 operator+(const v2 &v) const {
        return add(v);
    }

    v2 operator-(const v2 &v) const {
        return sub(v);
    }

    v2 operator*(float scalar) const {
        return mul(scalar);
    }

    v2 operator/(float scalar) const {
        return div(scalar);
    }


    float mag() const {
        return (float) std::sqrt(this->x * this->x + this->y * this->y);
    }

    v2 setX(float newX) const {
        return {newX, this->y};
    }

    v2 setY(float newY) const {
        return {newY, this->y};
    }

    v2 floor() const {
        return {(float) std::floor(this->x), (float) std::floor(this->y)};
    }

    v2 round() const {
        return {(float) std::round(this->x), (float) std::round(this->y)};
    }

    v2 ceil() const {
        return {(float) std::ceil(this->x), (float) std::ceil(this->y)};
    }

    v2 norm() const {
        v2 vec = {this->x, this->y};
        return copy(vec.div(this->mag()));
    }

    bool eq(v2 other) const {
        bool result = this->x == other.x && this->y == other.y;
        return result;
    }

    v2 updt(v2 to) {
        this->x = to.x;
        this->y = to.y;

        return *this;
    }

    v2 rotate_around(v2 pivot, float degrees) const {
        float radians = (degrees * pi) / 180;
        float dx = this->x - pivot.x;
        float dy = this->y - pivot.y;

        float cos = std::cos(radians);
        float sin = std::sin(radians);

        float rx = cos * dx - sin * dy + pivot.x;
        float ry = sin * dx + cos * dy + pivot.y;

        return {rx, ry};
    }

    std::string to_string() const {
        return "v2(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }

    bool operator ==(v2 other) const {
        return this->x == other.x && this->y == other.y;
    }
};
