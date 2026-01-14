#pragma once

#include "threading_provider.hpp"
#include <functional>
#include <memory>
#include <thread>

class desktop_thread : public provided_thread {
private:
  std::thread thread;

public:
  desktop_thread(std::function<void()> action) { thread = std::thread(action); }

  void stop() override {
    if (thread.joinable()) {
      if (std::this_thread::get_id() == thread.get_id()) {
        thread.detach();
      } else {
        thread.join();
      };
    };
  };

  void sleep_for(long ms) override {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }
};

class desktop_threading_provider : public threading_provider {
  std::shared_ptr<provided_thread>
  start(std::function<void()> action) override {
    return std::make_shared<desktop_thread>(action);
  };
};
