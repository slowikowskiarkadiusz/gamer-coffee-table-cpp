#pragma once

#include <coroutine>
#include <thread>

struct wait_for_ms {
    int millisecons;

    explicit wait_for_ms(int s) : millisecons(s) {
    }

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<>) const {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(millisecons)));
    }

    void await_resume() const noexcept {
    }
};
