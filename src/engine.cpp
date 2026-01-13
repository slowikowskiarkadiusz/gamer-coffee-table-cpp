#include "engine.hpp"

#include "scenes/controls/controls_scene.hpp"
#include "scenes/esp32test/esp32test_scene.hpp"
#include "scenes/menu/menu_scene.hpp"
#include "scenes/pong/pong_scene.hpp"
#include "scenes/tetris/tetris_scene.hpp"

engine *engine::instance_ptr = nullptr;
float engine::delta_time = 0;
float engine::fixed_delta_time = 0;
v2 engine::screen_size = v2(64, 64);

void engine::run() {
  running = true;

  update_thread = threading_provider_->start([&]() {
    while (running) {
      long new_time = now_ms();
      delta_time = static_cast<float>(new_time - lastTimestamp);
      lastTimestamp = new_time;

      if (!current_scene) {
        open_scene(std::make_shared<esp32test_scene>());
      }

      input_.update(delta_time);
      gesture_handler.update(delta_time);

      check_go_back_to_menu(delta_time);

      current_scene->update(delta_time);

      for (auto &a : current_scene->actors) {
        if (a != nullptr) {
          a->update(delta_time);
        }
      }

      run_asyncable(delta_time);

      screen.clear();
      for (std::shared_ptr<actor> a : current_scene->actors) {
        screen.write(a->render(), a->get_center(), a->get_rotation(),
                     a->get_anchor_offset(), true, a->name());
      }

      if (on_frame_finished) {
        on_frame_finished(&screen.pixels);
      }

      input_.late_update(delta_time);
      gesture_handler.late_update(delta_time);

      update_thread->sleep_for(33 - (lastTimestamp - now_ms()));
    }
  });

  fixed_update_thread = threading_provider_->start([&]() {
    while (running) {
      long new_time = now_ms();
      fixed_delta_time = static_cast<float>(new_time - lastFixedTimestamp);
      lastFixedTimestamp = new_time;

      if (current_scene) {
        current_scene->fixed_update(fixed_delta_time);
        for (auto a : current_scene->actors) {
          a->fixed_update(fixed_delta_time);
        }
      }

      fixed_update_thread->sleep_for(16 - (lastTimestamp - now_ms()));
    }
  });
}

void engine::check_go_back_to_menu(float dt) {
  if (input_.is_key_press(key::START)) {
    goBackToMenuTimer += dt;
  }

  if (goBackToMenuTimer > 3000.0f) {
    goBackToMenuTimer = 0;
    open_scene(std::make_shared<menu_scene>());
  }
}
