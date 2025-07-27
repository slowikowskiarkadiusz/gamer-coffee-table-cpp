//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once
#include <thread>
#include <sys/types.h>

#include "actor.hpp"

class engine {
private:
    long lastTimestamp = 0;
    long lastFixedTimestamp = 0;
    // Input input;
    // Gestures gestures;
    // Scene scene;
    std::vector<actor> tempActors;
    // any[] timeouts;
    // any[] intervals;
    // any[] engineIntervals;
    float goBackToMenuTimer = 0;

    std::vector<std::thread> engine_threads = {};

public:
    static engine *inst;

    engine() {
        lastTimestamp = lastFixedTimestamp = getNowMs();
        inst = this;
    }

    v2 screenSize = v2::getOne().mul(32);
    matrix screen = new matrix(this.screenSize.x, this.screenSize.y);
    float deltaTime = 0;
    float fixedDeltaTime = 0;

    void run() {
        engine_threads.emplace_back([&]() {

        });
    }

    void registerActor();

    void unregisterActor();

    void openScene();

    void setTimeout();

    void clearTimeout();

    void setInterval();

    void clearInterval();

private:
    static long getNowMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }
};
