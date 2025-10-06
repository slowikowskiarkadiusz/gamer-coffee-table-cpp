#include "engine.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "scenes/pong/pong_scene.hpp"

static const char *TAG = "ENGINE";

engine *engine::instance_ptr = nullptr;
float engine::delta_time = 0.0f;
float engine::fixed_delta_time = 0.0f;
v2 engine::screen_size = v2(64, 64);

void engine::run() {
    running = true;

    xTaskCreatePinnedToCore(
        [](void *arg) {
            auto *self = static_cast<engine *>(arg);
            self->update_loop();
        },
        "engine_update", 8192, this, 5, &update_task_handle, 1 // rdzeń 1
    );

    xTaskCreatePinnedToCore(
        [](void *arg) {
            auto *self = static_cast<engine *>(arg);
            self->fixed_loop();
        },
        "engine_fixed", 4096, this, 4, &fixed_task_handle, 0 // rdzeń 0
    );
}

void engine::update_loop() {
    int64_t last = esp_timer_get_time(); // µs
    while (running) {
        int64_t now = esp_timer_get_time();
        delta_time = float((now - last) / 1000.0); // ms
        last = now;

        if (!current_scene) {
            open_scene(std::make_shared<pong_scene>());
        }

        input_.update(delta_time);
        gesture_handler.update(delta_time);

        current_scene->update(delta_time);
        for (auto &a: current_scene->actors)
            if (a) a->update(delta_time);

        run_asyncable(delta_time);

        screen.clear();
        for (auto &a: current_scene->actors)
            screen.write(a->render(), a->get_center(), a->get_rotation(), a->get_anchor_offset());

        if (on_frame_finished) {
            on_frame_finished(screen.pixels());
        }

        input_.late_update(delta_time);
        gesture_handler.late_update(delta_time);

        vTaskDelay(pdMS_TO_TICKS(16)); // ~60Hz
    }

    ESP_LOGW(TAG, "Update loop stopped.");
    vTaskDelete(nullptr);
}

void engine::fixed_loop() {
    int64_t last = esp_timer_get_time();
    while (running) {
        int64_t now = esp_timer_get_time();
        fixed_delta_time = float((now - last) / 1000.0);
        last = now;

        if (current_scene) {
            current_scene->fixed_update(fixed_delta_time);
            for (auto &a: current_scene->actors)
                if (a) a->fixed_update(fixed_delta_time);
        }

        vTaskDelay(pdMS_TO_TICKS(16)); // 60Hz fizyka
    }

    ESP_LOGW(TAG, "Fixed loop stopped.");
    vTaskDelete(nullptr);
}
