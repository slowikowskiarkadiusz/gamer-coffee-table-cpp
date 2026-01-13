#pragma once

#include <functional>

class provided_thread {
protected:
public:
  virtual void stop() = 0;
  virtual void sleep_for(long ms) = 0;
};

class threading_provider {
public:
  virtual std::shared_ptr<provided_thread>
  start(std::function<void()> action) = 0;
};
