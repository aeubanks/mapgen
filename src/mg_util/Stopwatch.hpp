#pragma once

#include "mg_util_include.hpp"

#include <chrono>


namespace mg_util {
class Stopwatch {
  public:
    static constexpr auto now = std::chrono::steady_clock::now;

    using time_point_t =
        decltype(now()); // std::chrono::time_point<std::chrono::steady_clock>;
    using time_dif_t =
        decltype(std::chrono::operator-(time_point_t(), time_point_t()));
    using time_dif_count_t = decltype(time_dif_t().count());
    using millis_t = std::chrono::milliseconds;

    Stopwatch();

    // update prev_time_ to now()
    void tick();
    // print now() - prev_time, then update prev_time_ to now()
    void tick_and_print_millis();
    // print now() - prev_time with a description, then update prev_time_ to now()
    void tick_and_print_millis(std::string & str);
    // print now() - begin_time_, then update prev_time_ to now()
    void print_total_time_millis();
    // print now() - begin_time_ with a description, then update prev_time_ to
    // now()
    void print_total_time_millis(std::string & str);
    // the total time in milliseconds since the Stopwatch was constructed
    time_dif_count_t total_time();
    // the time in milliseconds since the last tick
    time_dif_count_t time();

  private:
    static millis_t cast_to_millis(time_dif_t t);

    time_point_t begin_time_, prev_time_;
};
}

