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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "v2.hpp"
#include "matrix.hpp"
#include "scene.hpp"
#include "color.hpp"
#include "now_ms.hpp"
#include "input/gestures.hpp"
#include "input/input.hpp"
#include "grid2d.hpp"

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

    TaskHandle_t update_task_handle = nullptr;
    TaskHandle_t fixed_task_handle = nullptr;

    std::thread update_thread;
    std::thread fixed_update_thread;
    std::atomic<bool> running = false;

    std::function<void(const grid2d<color> &)> on_frame_finished;

    input input_;
    gestures gesture_handler;

    static engine &instance() {
        return *instance_ptr;
    }

public:
    static engine *instance_ptr;

    static v2 screen_size;
    matrix screen = matrix(screen_size.x, screen_size.y);
    static float delta_time;
    static float fixed_delta_time;

    explicit engine(std::shared_ptr<input_provider> input_provider): input_(std::move(input_provider)) {
        lastTimestamp = lastFixedTimestamp = now_ms();
        instance_ptr = this;
    }

    static void set_on_frame_finished(const std::function<void(const grid2d<color> &)> &callback) {
        instance().on_frame_finished = callback;
    }

    void run();
    void update_loop();
    void fixed_loop();

    void stop() {
        running = false;
        if (update_task_handle) vTaskDelete(update_task_handle);
        if (fixed_task_handle) vTaskDelete(fixed_task_handle);
    }

    static void register_actor(std::shared_ptr<actor> a) {
        instance().current_scene->actors.push_back(a);
        std::sort(instance_ptr->current_scene->actors.begin(), instance_ptr->current_scene->actors.end(), [](std::shared_ptr<actor> first, std::shared_ptr<actor> second) {
            return second->get_render_importance() > first->get_render_importance();
        });
    }

    static void unregister_actor(std::shared_ptr<actor> a) {
        std::erase(instance().current_scene->actors, a);
        a.reset();
    }

    static void open_scene(std::shared_ptr<scene> scene_ptr) {
        instance().timeouts.clear();
        instance().intervals.clear();

        if (instance().current_scene)
            instance().current_scene->actors.clear();

        instance().current_scene = std::move(scene_ptr);
        instance().current_scene->init();
    }

    static asyncable *set_timeout(const std::function<void()> &callback, float ms) {
        auto obj = new asyncable({callback, ms});
        instance().pending_timeouts.push_back(obj);
        return obj;
    }

    static asyncable *set_interval(const std::function<void()> &callback, float ms) {
        auto obj = new asyncable({callback, ms});
        instance().pending_intervals.push_back(obj);
        return obj;
    }

    static void *clear_timeout(asyncable *asyncable) {
        if (asyncable != nullptr && instance_ptr->timeouts.size() > 0) {
            const auto it = std::ranges::find(instance_ptr->timeouts, asyncable);
            instance_ptr->timeouts.erase(it);
        }

        return 0;
    }

    static void *clear_interval(asyncable *asyncable) {
        if (asyncable != nullptr && instance_ptr->intervals.size() > 0) {
            const auto it = std::ranges::find(instance_ptr->intervals, asyncable);
            instance_ptr->intervals.erase(it);
        }

        return 0;
    }

    template<class T, class... args>
    static std::shared_ptr<T> instantiate(args &&... _args) {
        auto obj = std::make_shared<T>(std::forward<args>(_args)...);
        instance().register_actor(obj);
        return obj;
    }

    ~engine() {
        if (update_thread.joinable()) {
            if (std::this_thread::get_id() == update_thread.get_id()) {
                update_thread.detach(); // nigdy nie joinuj samego siebie
            } else {
                update_thread.join();
            }
        }

        if (fixed_update_thread.joinable()) {
            if (std::this_thread::get_id() == fixed_update_thread.get_id()) {
                fixed_update_thread.detach();
            } else {
                fixed_update_thread.join();
            }
        }
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
