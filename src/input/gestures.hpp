#pragma once
#include <format>
#include <map>
#include <__ranges/transform_view.h>

#include "input.hpp"
#include "key.hpp"
#include "../max_heap.hpp"
#include "../engine.hpp"

const float max_gesture_delay = 200;
const float long_press_duration = 300;
const float repeater_press_duration = 150;
// const keysLength = Math.max(...Object.values(Key).filter(v => typeof v === "number")) as number;

enum class state {
    down,
    up,
    press,
};

enum class gesture {
    _single = 1,
    _double = 2,
    _triple = 3,
    _long = 4,
    _repeater = 5,
};

class gestures {
    std::unordered_map<state, std::function<bool(key)> > states = {
        {state::down, [](key key) { return input::instance().is_key_down(key); }},
        {state::up, [](key key) { return input::instance().is_key_up(key); }},
        {state::press, [](key key) { return input::instance().is_key_press(key); }},
    };
    std::vector<std::map<state, max_heap *> > last_action_timestamps;
    // private lastActionTimestamps: { [state in State]?: MaxHeap }[] = Array.from({ length: maxGestureDelay }, () => {return {}});
    std::unordered_map<std::string, bool> gestures_this_frame;
    std::unordered_map<std::string, gesture> single_gestures_this_frame;
    std::unordered_map<std::string, std::thread> awaiting_for_bigger;
    std::unordered_map<std::string, int> press_timers;
    std::unordered_map<std::string, int> repeater_timers;

    gestures() {
        std::vector<std::map<state, max_heap *> > last_action_timestamps = std::vector<std::map<state, max_heap *> >(keys_length);
    }

    std::string make_key(key key, state state, std::optional<gesture> gesture) {
        auto result = std::format("{0}_{1}", key, state);

        if (gesture.has_value())
            result += std::format("_{0}", gesture.value());

        return result;
    }

public:
    bool is(key key, state state, gesture gesture, bool is_single_gesture) {
        if (is_single_gesture)
            return single_gestures_this_frame[make_key(key, state, std::nullopt)] == gesture;
        else
            return gestures_this_frame[make_key(key, state, gesture)];
    }

    bool is(std::vector<key> keys, state state, gesture gesture, bool is_single_gesture) {
        for (int i = 0; i < keys.size(); ++i)
            if (!is(keys[i], state, gesture, is_single_gesture))
                return false;

        return true;
    }

    void update(float delta_time) {
        auto now = engine::now_ms();

        for (int keyI = 0; keyI < keys_length; ++keyI) {
            auto key_value = static_cast<key>(keyI);

            for (const auto &[state, predicate]: states) {
                if (last_action_timestamps[keyI][state] != nullptr) {
                    *last_action_timestamps[keyI][state] = max_heap(3);
                }

                if (predicate(key_value)) {
                    last_action_timestamps[keyI][state]->insert(now);

                    for (int gestureI = static_cast<int>(gesture::_triple); gestureI >= static_cast<int>(gesture::_single); gestureI--) {
                        auto c = last_action_timestamps[keyI][state]->values();
                        bool doesQualify = c.size() >= gestureI;

                        for (int i = 0; i < std::min(gestureI - 1, (int) c.size() - 1); ++i) {
                            if (c[i] - c[i + 1] > max_gesture_delay) {
                                doesQualify = false;
                                break;
                            }
                        }


                        if (doesQualify) {
                            // gestures_this_frame[make_key(keyI, state, static_cast<gesture>(gestureI))]
                        }
                    }
                }
            }
        }
    }
};
