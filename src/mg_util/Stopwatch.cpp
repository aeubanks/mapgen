//
// Created by Arthur Eubanks on 24/12/15.
//

#include "Stopwatch.hpp"

#include "fmt/format.hpp"

namespace mg_util {
Stopwatch::Stopwatch() : begin_time_(now()), prev_time_(now()) {}

Stopwatch::millis_t Stopwatch::cast_to_millis(time_dif_t t) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t);
}

void Stopwatch::tick() { prev_time_ = now(); }

Stopwatch::time_dif_count_t Stopwatch::time() {
    return cast_to_millis(now() - prev_time_).count();
}

Stopwatch::time_dif_count_t Stopwatch::total_time() {
    return cast_to_millis(now() - begin_time_).count();
}

void Stopwatch::tick_and_print_millis() {
    fmt::print("time: {} millis\n", time());
    tick();
}

void Stopwatch::tick_and_print_millis(string & str) {
    fmt::print("{} time: {} millis\n", str, time());
    tick();
}

void Stopwatch::print_total_time_millis() {
    fmt::print("total time: {} millis\n", total_time());
}

void Stopwatch::print_total_time_millis(string & str) {
    fmt::print("{} total time: {} millis\n", str, total_time());
}
}
