#pragma once

#include <functional>
#include <chrono>

class Timer {
  public:
    void reset() { elapsed_ = elapsed_.zero(); }
    auto split() { return elapsed_; }
    void start();
    void stop();
    auto elapsed() { return elapsed_; }
  private:
    std::chrono::high_resolution_clock::time_point t0, t1;
    std::chrono::nanoseconds elapsed_{0};
};

void
Timer::start()
{
  t0 = std::chrono::high_resolution_clock::now();
}

void
Timer::stop()
{
  auto t = std::chrono::high_resolution_clock::now();
  elapsed_ += t - t0;
  //t0 = t;
}

template<typename F>
  auto benchmark(F func, std::size_t reps)
  {
    Timer timer;
    timer.start();
    for (std::size_t i = 0; i < reps; ++i) {
      func();
    }
    timer.stop();
    return timer.elapsed().count();
  }