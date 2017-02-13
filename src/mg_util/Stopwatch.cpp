//
// Created by Arthur Eubanks on 24/12/15.
//

#include "Stopwatch.hpp"

#include "mg_log/Log.hpp"

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
    mg_log::info("time: ", time(), " millis");
    tick();
}

void Stopwatch::tick_and_print_millis(std::string & str) {
    mg_log::info(str, " time: ", time(), " millis");
    tick();
}

void Stopwatch::print_total_time_millis() {
    mg_log::info("total time: ", total_time(), " millis");
}

void Stopwatch::print_total_time_millis(std::string & str) {
    mg_log::info(str, " total time: ", total_time(), " millis");
}
}
