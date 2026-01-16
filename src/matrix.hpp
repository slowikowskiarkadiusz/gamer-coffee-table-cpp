#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "v2.hpp"
#include "color.hpp"
#include "grid2d.hpp"

enum class color_blending
{
    no_blending,
    light_alike,
    weighted_by_opacity,
};

class matrix
{
public:
    grid2d<color> pixels;

    matrix(v2 size, const color &default_color = color::none(), std::string debug_name = "") : matrix(size.x, size.y, default_color, debug_name)
    {
    }

    matrix(int x, int y, const color &default_color = color::none(), std::string debug_name = "")
    {
        x = std::round(x);
        y = std::round(y);
        pixels = grid2d<color>(x, y, default_color, "matrix " + debug_name);
    }

    const uint width() const
    {
        return pixels.width();
    }

    const uint height() const
    {
        return pixels.height();
    }

    void set_pixel_v2(const v2 &point, const color &color)
    {
        pixels.at(point.x, point.y) = color.copy();
    }

    void set_pixel(int x, int y, const color &color)
    {
        pixels.at(x, y) = color.copy();
    }

    matrix fill(color &color)
    {
        pixels.fill(color);
        return *this;
    }

    matrix &write_at_origin(const matrix &other, const v2 &origin)
    {
        for (size_t x = 0; x < other.pixels.width(); x++)
        {
            for (size_t y = 0; y < other.pixels.height(); y++)
            {
                int tx = x + origin.x;
                int ty = y + origin.y;
                if (tx >= 0 && tx < pixels.width() && ty >= 0 && ty < pixels.height())
                {
                    pixels.at(tx, ty) = other.pixels.at(x, y).copy();
                }
            }
        }
        return *this;
    }

    matrix &write(const matrix *other,
                  const v2 &otherCenter,
                  float otherRotation = 0.0f,
                  const v2 &otherAnchor = v2::zero(),
                  bool blend_colors = true,
                  std::string name = "")
    {
        auto over_blend = [](const color &dst, const color &src) -> color
        {
            float sa = src.a, da = dst.a;
            float outA = sa + da * (1.0f - sa);
            if (outA <= 0.0f)
                return color(0, 0, 0, 0);
            float r = (src.r * sa + dst.r * da * (1.0f - sa)) / outA;
            float g = (src.g * sa + dst.g * da * (1.0f - sa)) / outA;
            float b = (src.b * sa + dst.b * da * (1.0f - sa)) / outA;
            return color(r, g, b, outA);
        };

        int otherWidth = other->pixels.width();
        int otherHeight = other->pixels.height();
        if (otherWidth == 0 || otherHeight == 0)
            return *this;
        v2 center(otherWidth / 2.0f, otherHeight / 2.0f);
        int angle = static_cast<int>(otherRotation) % 360;
        if (angle < 0)
            angle += 360;

        bool useTrig = (angle % 90) != 0;
        float rad = otherRotation * M_PI / 180.0f;
        float cs = std::cos(rad), sn = std::sin(rad);

        for (int y = 0; y < otherHeight; y++)
        {
            for (int x = 0; x < otherWidth; x++)
            {
                color src = other->pixels.at(x, y);
                if (src.is_none())
                    continue;

                float dx = x - center.x + otherAnchor.x;
                float dy = y - center.y + otherAnchor.y;

                float rx, ry;
                if (useTrig)
                {
                    rx = dx * cs - dy * sn;
                    ry = dx * sn + dy * cs;
                }
                else
                {
                    switch (angle)
                    {
                    case 0:
                        rx = dx;
                        ry = dy;
                        break;
                    case 90:
                        rx = -dy;
                        ry = dx;
                        break;
                    case 180:
                        rx = -dx;
                        ry = -dy;
                        break;
                    case 270:
                        rx = dy;
                        ry = -dx;
                        break;
                    default:
                        rx = dx;
                        ry = dy;
                        break;
                    }
                }

                int finalX = static_cast<int>(std::ceil(rx + otherCenter.x));
                int finalY = static_cast<int>(std::ceil(ry + otherCenter.y));

                if (finalX < 0 || finalY < 0 ||
                    finalX >= pixels.width() || finalY >= pixels.height())
                    continue;

                color &dst = pixels.at(finalX, finalY);

                if (blend_colors)
                {
                    dst = over_blend(dst, src);
                }
                else
                    dst = src;
            }
        }

        return *this;
    }

    matrix &rotate(float degrees)
    {
        float radians = (degrees * M_PI) / 180.0f;
        float sinA = std::abs(std::sin(radians));
        float cosA = std::abs(std::cos(radians));

        int old_width = pixels.width();
        int old_height = pixels.height();
        int new_width = std::ceil(old_width * cosA + old_height * sinA);
        int new_height = std::ceil(old_width * sinA + old_height * cosA);

        matrix rotated(new_width, new_height, color::none(), "rotate");

        float cxOld = old_width / 2.0f;
        float cyOld = old_height / 2.0f;
        float cxNew = new_width / 2.0f;
        float cyNew = new_height / 2.0f;

        for (int x = 0; x < old_width; x++)
        {
            for (int y = 0; y < old_height; y++)
            {
                float dx = x - cxOld;
                float dy = y - cyOld;

                int rx = std::round(std::cos(radians) * dx - std::sin(radians) * dy + cxNew);
                int ry = std::round(std::sin(radians) * dx + std::cos(radians) * dy + cyNew);

                if (rx >= 0 && rx < new_width && ry >= 0 && ry < new_height)
                {
                    rotated.pixels.at(rx, ry) = pixels(x, y);
                }
            }
        }

        pixels = rotated.pixels;
        return *this;
    }

    matrix &scale(const float factor)
    {
        int old_width = pixels.width();
        int old_height = pixels.height();
        int new_width = std::round(old_width * factor);
        int new_height = std::round(old_height * factor);

        matrix scaled(new_width, new_height, color::none(), "scale");

        for (int x = 0; x < new_width; x++)
        {
            for (int y = 0; y < new_height; y++)
            {
                int srcX = std::floor(x / factor);
                int srcY = std::floor(y / factor);
                if (srcX < old_width && srcY < old_height)
                {
                    scaled.pixels(x, y) = pixels(srcX, srcY);
                }
            }
        }

        pixels = scaled.pixels;
        return *this;
    }

    matrix snippet(const v2 &from, const v2 &to) const
    {
        matrix result(to.x - from.x, to.y - from.y, color::none(), "snippet");
        for (int x = from.x; x < to.x; x++)
        {
            for (int y = from.y; y < to.y; y++)
            {
                result.pixels.at(x - from.x, y - from.y) = pixels.at(x, y);
            }
        }
        return result;
    }

    matrix &clear()
    {
        for (size_t y = 0; y < pixels.height(); y++)
        {
            for (size_t x = 0; x < pixels.width(); x++)
                pixels.at(x, y) = color::none();
        }
        return *this;
    }

    matrix &print_to_console()
    {
        std::string result = "\n";
        for (size_t y = 0; y < pixels.height(); y++)
        {
            result += "|";
            for (size_t x = 0; x < pixels.width(); x++)
            {
                result += pixels.at(x, y).is_none() ? "  " : "O ";
            }
            result += "|\n";
        }

        std::cout << result << std::endl;

        return *this;
    }
};
