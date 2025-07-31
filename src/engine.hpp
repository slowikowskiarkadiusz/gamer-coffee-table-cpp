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

struct asyncable {
    std::function<void()> function;
    float ms;
    float timer = 0;
};

struct coroutine {
    std::function<int()> function;
    float timer_left = 0;
};

class engine {
private:
    long lastTimestamp = 0;
    long lastFixedTimestamp = 0;
    float goBackToMenuTimer = 0;

    std::shared_ptr<scene> current_scene = nullptr;

    std::vector<asyncable *> timeouts;
    std::vector<asyncable *> pending_timeouts;
    std::vector<asyncable *> timeouts_to_clear;
    std::vector<asyncable *> intervals;
    std::vector<asyncable *> pending_intervals;
    std::vector<asyncable *> intervals_to_clear;

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
        timeouts.clear();
        intervals.clear();

        if (current_scene)
            current_scene->actors.clear();

        current_scene = std::move(scene_ptr);
        current_scene->init();
    }

    asyncable *set_timeout(const std::function<void()> &callback, float ms) {
        auto obj = new asyncable({callback, ms});
        pending_timeouts.push_back(obj);
        return obj;
    }

    asyncable *set_interval(const std::function<void()> &callback, float ms) {
        auto obj = new asyncable({callback, ms});
        pending_intervals.push_back(obj);
        return obj;
    }

    void *clear_timeout(asyncable *asyncable) {
        if (asyncable != nullptr) {
            const auto it = std::ranges::find(timeouts, asyncable);
            timeouts.erase(it);
        }

        return 0;
    }

    void *clear_interval(asyncable *asyncable) {
        if (asyncable != nullptr) {
            const auto it = std::ranges::find(intervals, asyncable);
            intervals.erase(it);
        }

        return 0;
    }

    template<class T, class... args>
    static std::shared_ptr<T> instantiate(args &&... _args) {
        auto obj = std::make_shared<T>(std::forward<args>(_args)...);
        instance().register_actor(obj);
        return obj;
    }

private:
    void check_go_back_to_menu(float dt);

    void run_asyncable(float delta_time) {
        if (!pending_timeouts.empty()) {
            timeouts.insert(timeouts.end(),
                            std::make_move_iterator(pending_timeouts.begin()),
                            std::make_move_iterator(pending_timeouts.end()));
            pending_timeouts.clear();
        }

        for (auto it = timeouts.begin(); it != timeouts.end();) {
            (*it)->timer += delta_time;

            if ((*it)->timer >= (*it)->ms) {
                (*it)->function();
                it = timeouts.erase(it);
            } else {
                ++it;
            }
        }

        if (!pending_intervals.empty()) {
            intervals.insert(intervals.end(),
                             std::make_move_iterator(pending_intervals.begin()),
                             std::make_move_iterator(pending_intervals.end()));
            pending_intervals.clear();
        }

        for (auto &e: intervals) {
            e->timer += delta_time;
            if (e->timer >= e->ms) {
                e->timer = 0;
                e->function();
            }
        }
    }
};
