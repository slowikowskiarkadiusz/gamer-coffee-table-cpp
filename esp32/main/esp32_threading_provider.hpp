#pragma once

#include "../../src/engine.cpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <functional>
#include <memory>

class esp32_thread : public provided_thread {
    std::function<void()> fn;
    TaskHandle_t handle = nullptr;

    static void task_entry(void* arg) {
        auto* self = static_cast<esp32_thread*>(arg);
        self->fn();
        vTaskDelete(nullptr);
    }

public:
    esp32_thread(std::function<void()> action)
        : fn(std::move(action)) {

        xTaskCreatePinnedToCore(
            task_entry,
            "engine_task",
            8192,
            this,
            5,
            &handle,
            1
        );
    }

    void sleep_for(long ms) override {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

    void stop() override {
        if (handle) {
            vTaskDelete(handle);
            handle = nullptr;
        }
    }
};

class esp32_threading_provider : public threading_provider {
  std::shared_ptr<provided_thread>
  start(std::function<void()> action) override {
    return std::make_shared<esp32_thread>(action);
  };
};
