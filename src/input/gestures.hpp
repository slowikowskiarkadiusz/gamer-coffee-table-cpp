#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <chrono>
#include <format>
#include <algorithm>
#include <iostream>

#include "input.hpp"
#include "key.hpp"
#include "../now_ms.hpp"
#include "../max_heap.hpp"

const float max_gesture_delay = 200;
const float long_press_duration = 300;
const float repeater_press_duration = 150;

enum class state { down, up, press };

enum class gesture { once = 1, twice = 2, trice = 3, prolonged = 4, repeating = 5 };

class gestures {
    static gestures *instance_ptr;

    std::unordered_map<state, std::function<bool(key)> > states = {
        {state::down, [](key k) { return input::is_key_down(k); }},
        {state::up, [](key k) { return input::is_key_up(k); }},
        {state::press, [](key k) { return input::is_key_press(k); }},
    };

    std::vector<std::map<state, max_heap *> > last_action_timestamps;
    std::unordered_map<std::string, bool> gestures_this_frame;
    std::unordered_map<std::string, gesture> single_gestures_this_frame;
    std::unordered_map<int, float> press_timers;
    std::unordered_map<int, float> repeater_timers;

    static gestures &instance() {
        return *instance_ptr;
    }

public:
    gestures() {
        last_action_timestamps.resize(keys_length);
        for (auto &map: last_action_timestamps)
            for (auto &[s, _]: states)
                map[s] = new max_heap(3);

        instance_ptr = this;
    }

    ~gestures() {
        for (auto &map: last_action_timestamps)
            for (auto &[_, ptr]: map)
                delete ptr;
    }

    static std::string make_key(key k, state s, std::optional<gesture> g = std::nullopt) {
        std::string res = std::format("{}_{}", static_cast<int>(k), static_cast<int>(s));
        if (g.has_value())
            res += std::format("_{}", static_cast<int>(g.value()));
        return res;
    }

    static bool is(key k, state s, gesture g, bool single = false) {
        auto id = make_key(k, s, single ? std::nullopt : std::optional(g));
        return single ? instance().single_gestures_this_frame[id] == g : instance().gestures_this_frame[id];
    }

    static bool is(const std::vector<key> &keys, state s, gesture g, bool single = false) {
        return std::all_of(keys.begin(), keys.end(), [&](key k) { return is(k, s, g, single); });
    }

    void update(float delta_time) {
        auto now = now_ms();

        for (int k = 0; k < keys_length; ++k) {
            auto key_val = static_cast<key>(k);

            for (auto &[s, pred]: states) {
                auto *heap = last_action_timestamps[k][s];
                if (pred(key_val)) {
                    heap->insert(now);

                    for (int g = static_cast<int>(gesture::trice); g >= static_cast<int>(gesture::once); --g) {
                        auto vals = heap->values();
                        if (vals.size() < g) continue;

                        bool valid = true;
                        for (int i = 0; i < g - 1; ++i) {
                            if (vals[i] - vals[i + 1] > max_gesture_delay) {
                                valid = false;
                                break;
                            }
                        }

                        if (!valid) continue;

                        auto g_key = make_key(key_val, s, static_cast<gesture>(g));
                        gestures_this_frame[g_key] = true;
                    }
                }

                if (s == state::press) {
                    int id = static_cast<int>(key_val);
                    auto val = pred(key_val) ? delta_time : -press_timers[id];
                    press_timers[id] += val;
                    repeater_timers[id] += val;

                    auto press_key = make_key(key_val, s);
                    if (press_timers[id] > long_press_duration) {
                        gestures_this_frame[make_key(key_val, s, gesture::prolonged)] = true;
                        single_gestures_this_frame[press_key] = gesture::prolonged;
                        press_timers[id] = 0;
                    }
                    if (repeater_timers[id] > repeater_press_duration) {
                        gestures_this_frame[make_key(key_val, s, gesture::repeating)] = true;
                        single_gestures_this_frame[press_key] = gesture::repeating;
                        repeater_timers[id] = 0;
                    }
                }
            }
        }
    }

    void late_update(float) {
        gestures_this_frame.clear();
        single_gestures_this_frame.clear();
    }
};
