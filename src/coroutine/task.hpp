#pragma once

#include <coroutine>
#include "wait_for_ms.hpp"

struct task {
    struct promise_type {
        task get_return_object() {
            return task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }

        void return_void() {
        }

        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    task(std::coroutine_handle<promise_type> h) : handle(h) {
    }

    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<>) { handle.resume(); }

    void await_resume() {
    }
};
