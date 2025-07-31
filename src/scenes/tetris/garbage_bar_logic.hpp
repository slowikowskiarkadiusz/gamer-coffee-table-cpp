#pragma once

#include <algorithm>
#include <memory>
#include "../../matrix.hpp"
#include "../../engine.hpp"

constexpr int garbageInsertionDelay = 4000;
constexpr int blinkingDelay = 100;

struct garbage_bar_opts {
    color color = color::white(0.1f);
};

class garbage_bar_logic {
private:
    matrix _matrix;
    matrix blinkOffmatrix;
    int currentLevel = 0;
    int animCurrentLevel = 0;
    int maxLevel;
    asyncable *currentAnim = nullptr;
    bool isReadyToDrop = false;
    asyncable *gettingReadyTimeout = nullptr;
    bool blinkOn = true;
    float blinkingTimer = 0;
    bool skipTextPop = false;
    garbage_bar_opts opts;

public:
    v2 size;
    v2 center;

    garbage_bar_logic(v2 center, v2 size, garbage_bar_opts opts = {})
        : center(center), size(size), opts(opts), _matrix(makeRectangle(size, opts.color)), blinkOffmatrix(makeRectangle(size, opts.color)) {
        if (opts.color.is_none())
            opts.color = color::white(0.5f);

        maxLevel = size.y;
    }

    void addLines(int noOfLines) {
        skipTextPop = false;
        engine::instance().clear_interval(currentAnim);
        currentAnim = anim(noOfLines);

        currentLevel += noOfLines;

        if (gettingReadyTimeout == nullptr) {
            gettingReadyTimeout = engine::instance().set_timeout([this]() {
                isReadyToDrop = true;
                gettingReadyTimeout = nullptr;
            }, garbageInsertionDelay);
        }
    }

    bool pop() {
        if (isReadyToDrop && currentLevel > 0 && !skipTextPop) {
            currentLevel--;
            if (currentLevel <= 0) {
                currentLevel = 0;
                isReadyToDrop = false;
                _matrix.write_at_origin(blinkOffmatrix, v2::zero());
            }
            return true;
        } else {
            skipTextPop = false;
            return false;
        }
    }

    int decreaseAndGetLeft(int by) {
        skipTextPop = true;
        int left = std::max(by - currentLevel, 0);
        currentLevel -= by;
        currentLevel = std::max(currentLevel, 0);
        animCurrentLevel = currentLevel;

        drawLevelOnmatrix();

        return left;
    }

    void update(float deltaTime) {
        if (isReadyToDrop && currentLevel > 0) {
            blinkingTimer += deltaTime;
            if (blinkingTimer > blinkingDelay) {
                blinkOn = !blinkOn;
                blinkingTimer = 0;
            }
        } else {
            blinkOn = true;
        }
    }

    matrix render() const {
        return blinkOn ? _matrix : blinkOffmatrix;
    }

private:
    asyncable *anim(int levelsToAdd) {
        return engine::instance().set_interval([this]() {
            drawLevelOnmatrix();
            animCurrentLevel++;
            drawLevelOnmatrix();
        }, 0);
    }

    void drawLevelOnmatrix() {
        color color = color::lerp(color::yellow(), color::red(), float(animCurrentLevel) / maxLevel);

        for (int y = 0; y < animCurrentLevel; ++y) {
            _matrix.set_pixel(0, _matrix.height() - 1 - y, color);
        }

        for (int y = animCurrentLevel; y < _matrix.height(); ++y) {
            _matrix.set_pixel(0, _matrix.height() - 1 - y, opts.color);
        }
    }

    matrix makeRectangle(v2 size, const color &color) {
        if (color.is_none())
            return matrix(std::abs(size.x), std::abs(size.y));
        return matrix(std::abs(size.x), std::abs(size.y), color);
    }
};
