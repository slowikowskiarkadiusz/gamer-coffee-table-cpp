#pragma once

#include <chrono>
#include <thread>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <future>
#include <unordered_map>
#include <algorithm>

#include "v2.hpp"
#include "matrix.hpp"
#include "scene.hpp"
#include "color.hpp"
#include "input/input.hpp"
#include "input/key.hpp"
#include "scenes/menu/menu-scene.hpp"

class actor;

class engine {
private:
    long lastTimestamp = 0;
    long lastFixedTimestamp = 0;
    float goBackToMenuTimer = 0;

    std::shared_ptr<scene<std::nullptr_t> > current_scene = nullptr;
    std::vector<std::shared_ptr<actor> > temp_actors;

    std::vector<std::future<void> > timeouts;
    std::vector<std::future<void> > intervals;

    std::thread update_thread;
    std::thread fixed_update_thread;
    std::atomic<bool> running = false;

    std::function<void(const std::vector<std::vector<color> > &)> on_frame_finished;

    input input;
    // gestures gesture_handler;

public:
    static engine *instance_ptr;

    v2 screen_size = v2::get_one().mul(32);
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

    void run() {
        running = true;

        update_thread = std::thread([&]() {
            while (running) {
                long new_time = now_ms();
                delta_time = static_cast<float>(new_time - lastTimestamp);
                lastTimestamp = new_time;

                if (!current_scene) {
                    open_scene(std::make_shared<menu_scene>());
                }

                input.update(delta_time);
                // gesture_handler.update(delta_time);

                check_go_back_to_menu(delta_time);

                current_scene->update(delta_time);
                for (auto &a: current_scene->actors) {
                    a->update(delta_time);
                }

                screen.clear();
                for (auto &a: current_scene->actors) {
                    screen.write(a->render(), a->center(), a->rotation());
                }

                if (on_frame_finished) {
                    on_frame_finished(screen.pixels());
                }

                input.late_update(delta_time);
                // gesture_handler.late_update(delta_time);

                std::this_thread::sleep_for(std::chrono::milliseconds(33));
            }
        });

        fixed_update_thread = std::thread([&]() {
            while (running) {
                long new_time = now_ms();
                fixed_delta_time = static_cast<float>(new_time - lastFixedTimestamp);
                lastFixedTimestamp = new_time;

                current_scene->fixed_update(fixed_delta_time);
                for (auto &a: current_scene->actors) {
                    a->fixed_update(fixed_delta_time);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        });
    }

    void stop() {
        running = false;
        if (update_thread.joinable()) update_thread.join();
        if (fixed_update_thread.joinable()) fixed_update_thread.join();
    }

    void register_actor(std::shared_ptr<actor> a) {
        if (!current_scene)
            temp_actors.push_back(a);
        else
            current_scene->actors.push_back(a);
    }

    void unregister_actor(std::shared_ptr<actor> a) {
        if (!current_scene) {
            temp_actors.erase(std::remove(temp_actors.begin(), temp_actors.end(), a), temp_actors.end());
        } else {
            current_scene->actors.erase(std::remove(current_scene->actors.begin(), current_scene->actors.end(), a),
                                        current_scene->actors.end());
        }
    }

    void open_scene(std::shared_ptr<scene<std::nullptr_t> > scene_ptr) {
        for (auto &f: timeouts) f.wait();
        for (auto &f: intervals) f.wait();

        if (current_scene)
            current_scene->actors.clear();

        current_scene = scene_ptr;
        current_scene->actors = temp_actors;
        temp_actors.clear();
    }

    void set_timeout(std::function<void()> callback, int ms) {
        timeouts.push_back(std::async(std::launch::async, [=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            callback();
        }));
    }

    void set_interval(std::function<void()> callback, int ms) {
        intervals.push_back(std::async(std::launch::async, [=]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
                callback();
            }
        }));
    }

private:
    long now_ms() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    void check_go_back_to_menu(float dt) {
        if (input.is_key_press(key::START)) {
            goBackToMenuTimer += dt;
        }

        if (goBackToMenuTimer > 3000.0f) {
            goBackToMenuTimer = 0;
            open_scene(std::make_shared<menu_scene>());
        }
    }
};
