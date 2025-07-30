#pragma once

#include <chrono>
#include <thread>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <future>
#include <algorithm>

#include "v2.hpp"
#include "matrix.hpp"
#include "scene.hpp"
#include "color.hpp"
#include "now_ms.hpp"
#include "input/gestures.hpp"
#include "input/input.hpp"

struct timeout {
    std::function<void()> function;
    long ms;
};

class engine {
private:
    long lastTimestamp = 0;
    long lastFixedTimestamp = 0;
    float goBackToMenuTimer = 0;

    std::shared_ptr<scene> current_scene = nullptr;

    std::vector<std::future<void> > timeouts;
    std::vector<std::future<void> > intervals;

    std::thread update_thread;
    std::thread fixed_update_thread;
    std::atomic<bool> running = false;

    std::function<void(const std::vector<std::vector<color> > &)> on_frame_finished;

    input input;
    gestures gesture_handler;

public:
    static engine *instance_ptr;

    v2 screen_size = v2(32, 32);
    matrix screen = matrix(screen_size.x, screen_size.y);
    float delta_time = 0;
    float fixed_delta_time = 0;

    explicit engine(std::shared_ptr<input_provider> input_provider): input(std::move(input_provider)) {
        lastTimestamp = lastFixedTimestamp = now_ms();
        instance_ptr = this;
    }

    static engine &instance() {
        return *instance_ptr;
    }

    void set_on_frame_finished(const std::function<void(const std::vector<std::vector<color> > &)> &callback) {
        on_frame_finished = callback;
    }

    void run();

    void stop() {
        running = false;
        if (update_thread.joinable()) update_thread.join();
        if (fixed_update_thread.joinable()) fixed_update_thread.join();
    }

    void register_actor(std::shared_ptr<actor> a) {
        current_scene->actors.push_back(a);
    }

    void unregister_actor(std::shared_ptr<actor> a) {
        std::erase(current_scene->actors, a);
    }

    void open_scene(std::shared_ptr<scene> scene_ptr) {
        for (auto &f: timeouts) f.wait();
        for (auto &f: intervals) f.wait();

        if (current_scene)
            current_scene->actors.clear();

        current_scene = std::move(scene_ptr);
        current_scene->init();
    }

    void set_timeout(const std::function<void()> &callback, int ms) {
        timeouts.push_back(std::async(std::launch::async, [=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            callback();
        }));
    }

    void set_interval(const std::function<void()> &callback, int ms) {
        intervals.push_back(std::async(std::launch::async, [&]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
                callback();
            }
        }));
    }

private:
    void check_go_back_to_menu(float dt);
};
