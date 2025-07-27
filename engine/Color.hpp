//
// Created by Arkadiusz on 27/07/2025.
//

#ifndef COLOR_HPP
#define COLOR_HPP

#endif //COLOR_HPP
#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

class Color {
public:
    float r, g, b, a;

    Color(float r = 0, float g = 0, float b = 0, float a = 1)
        : r(r), g(g), b(b), a(a) {
    }

    static Color red(float a = 1) { return Color(1, 0, 0, a); }
    static Color green(float a = 1) { return Color(0, 1, 0, a); }
    static Color blue(float a = 1) { return Color(0, 0, 1, a); }
    static Color yellow(float a = 1) { return Color(1, 1, 0, a); }
    static Color cyan(float a = 1) { return Color(0, 1, 1, a); }
    static Color magenta(float a = 1) { return Color(1, 0, 1, a); }
    static Color white(float a = 1) { return Color(1, 1, 1, a); }
    static Color black(float a = 1) { return Color(0, 0, 0, a); }
    static Color none() { return Color::black(0); }

    static Color blendColors(const std::vector<Color> &inputColors) {
        std::vector<Color> colors;
        for (const auto &c: inputColors) {
            if (!c.isNone()) colors.push_back(c);
        }
        if (colors.empty()) return Color::none();
        if (colors.size() == 1) return colors[0].copy();
        return additiveBlending(colors);
    }

    static Color lerp(const Color &from, const Color &to, float step) {
        float r = from.r + (to.r - from.r) * step;
        float g = from.g + (to.g - from.g) * step;
        float b = from.b + (to.b - from.b) * step;
        float a = from.a + (to.a - from.a) * step;
        return Color(std::min(r, 1.0f), std::min(g, 1.0f), std::min(b, 1.0f), std::min(a, 1.0f));
    }

    Color copy() const {
        return Color(r, g, b, a);
    }

    bool isNone() const {
        return r == 0 && g == 0 && b == 0 && a == 0;
    }

    bool equals(const Color &other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    std::string toRGBAString() const {
        std::ostringstream oss;
        oss << "rgba("
                << static_cast<int>(r * 255) << ", "
                << static_cast<int>(g * 255) << ", "
                << static_cast<int>(b * 255) << ", "
                << a << ")";
        return oss.str();
    }

private:
    static Color additiveBlending(const std::vector<Color> &colors) {
        float r = 0, g = 0, b = 0, totalAlpha = 0;
        for (const auto &c: colors) {
            r += c.r * c.a;
            g += c.g * c.a;
            b += c.b * c.a;
            totalAlpha += c.a;
        }
        return Color(std::min(1.0f, r), std::min(1.0f, g), std::min(1.0f, b), std::min(1.0f, totalAlpha));
    }

    static Color subtractiveBlending(const std::vector<Color> &colors) {
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
        return Color(r, g, b, 1 - transmission);
    }
};
