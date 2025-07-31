#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

class color {
public:
    float r, g, b, a;

    color(float r = 0, float g = 0, float b = 0, float a = 1)
        : r(r), g(g), b(b), a(a) {
    }

    static color red(float a = 1) { return color(1, 0, 0, a); }
    static color green(float a = 1) { return color(0, 1, 0, a); }
    static color blue(float a = 1) { return color(0, 0, 1, a); }
    static color yellow(float a = 1) { return color(1, 1, 0, a); }
    static color cyan(float a = 1) { return color(0, 1, 1, a); }
    static color magenta(float a = 1) { return color(1, 0, 1, a); }
    static color white(float a = 1) { return color(1, 1, 1, a); }
    static color black(float a = 1) { return color(0, 0, 0, a); }
    static color none() { return color::black(0); }

    static color blend_colors(const std::vector<color> &inputColors) {
        std::vector<color> colors;
        for (const auto &c: inputColors) {
            if (!c.is_none()) colors.push_back(c);
        }
        if (colors.empty()) return color::none();
        if (colors.size() == 1) return colors[0].copy();
        return additive_blending(colors);
    }

    static color lerp(const color &from, const color &to, float step) {
        float r = from.r + (to.r - from.r) * step;
        float g = from.g + (to.g - from.g) * step;
        float b = from.b + (to.b - from.b) * step;
        float a = from.a + (to.a - from.a) * step;
        return color(std::min(r, 1.0f), std::min(g, 1.0f), std::min(b, 1.0f), std::min(a, 1.0f));
    }

    color copy() const {
        return color(r, g, b, a);
    }

    bool is_none() const {
        return r == 0 && g == 0 && b == 0 && a == 0;
    }

    bool equals(const color &other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    std::string to_rgba_string() const {
        std::ostringstream oss;
        oss << "rgba("
                << static_cast<int>(r * 255) << ", "
                << static_cast<int>(g * 255) << ", "
                << static_cast<int>(b * 255) << ", "
                << a << ")";
        return oss.str();
    }

    std::string to_string() const {
        return std::format("color({0}, {1}, {2}, {3})", r, g, b, a);
    }

private:
    static color additive_blending(const std::vector<color> &colors) {
        float r = 0, g = 0, b = 0, totalAlpha = 0;
        for (const auto &c: colors) {
            r += c.r * c.a;
            g += c.g * c.a;
            b += c.b * c.a;
            totalAlpha += c.a;
        }
        return color(std::min(1.0f, r), std::min(1.0f, g), std::min(1.0f, b), std::min(1.0f, totalAlpha));
    }

    static color subtractive_blending(const std::vector<color> &colors) {
        float r = 1, g = 1, b = 1, transmission = 1;
        for (const auto &c: colors) {
            float alpha = c.a;
            float absorbR = 1 - c.r;
            float absorbG = 1 - c.g;
            float absorbB = 1 - c.b;
            r *= 1 - absorbR * alpha;
            g *= 1 - absorbG * alpha;
            b *= 1 - absorbB * alpha;
            transmission *= (1 - alpha);
        }
        return color(r, g, b, 1 - transmission);
    }
};
