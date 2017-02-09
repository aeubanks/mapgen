
#include "Random.hpp"

#include <chrono>

#include "mg_util_global.hpp"

#include "mg_log/Log.hpp"

namespace mg_util {
Random::Random() : generator_(static_cast<decltype(generator_)::result_type>(std::chrono::system_clock::now().time_since_epoch().count())) {}

Random::Random(IntType seed) : generator_(static_cast<decltype(generator_)::result_type>(seed)) {}

bool Random::rand_bool() { return rand_bool(0.5); }

bool Random::rand_bool(double chance) {
    return std::bernoulli_distribution(chance)(generator_);
}

Random::IntType Random::rand_int_exc(IntType range) { return rand_int_inc(0, range - 1); }

Random::IntType Random::rand_int_exc(IntType lo, IntType hi) { return rand_int_inc(lo, hi - 1); }

Random::IntType Random::rand_int_inc(IntType range) { return rand_int_inc(0, range); }

Random::IntType Random::rand_int_inc(IntType lo, IntType hi) {
    if constexpr (DEBUG) {
        if (lo > hi) {
            mg_log::error("invalid arguments to Random::rand_int_inc");
            throw mg_error("");
        }
    }
    return std::uniform_int_distribution<IntType>(lo, hi)(generator_);
}

Random::IntType Random::rand_binomial(IntType tries, double prob) {
    return std::binomial_distribution<IntType>(tries, prob)(generator_);
}

double Random::rand_double() { return rand_double(0.0, 1.0); }

double Random::rand_double(double range) { return rand_double(0.0, range); }

double Random::rand_double(double lo, double hi) {
    return std::uniform_real_distribution<double>(lo, hi)(generator_);
}

double Random::rand_normal() { return rand_normal(0.0, 1.0); }

double Random::rand_normal(double mean, double stddev) {
    return std::normal_distribution<double>(mean, stddev)(generator_);
}
}
