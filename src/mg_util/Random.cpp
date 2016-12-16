//
// Created by Arthur Eubanks on 20/12/15.
//

#include "Random.hpp"

#include <chrono>

namespace mg_util {
	Random::Random() : generator_(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) { }

	Random::Random(int seed) : generator_(static_cast<unsigned int>(seed)) { }

	bool Random::rand_bool() {
		return rand_bool(0.5);
	}

	bool Random::rand_bool(double chance) {
		return std::bernoulli_distribution(chance)(generator_);
	}

	int Random::rand_int_exc(int range) {
		return rand_int_inc(0, range - 1);
	}

	int Random::rand_int_exc(int lo, int hi) {
		return rand_int_inc(lo, hi - 1);
	}

	int Random::rand_int_inc(int range) {
		return rand_int_inc(0, range);
	}

	int Random::rand_int_inc(int lo, int hi) {
		return std::uniform_int_distribution<int>(lo, hi)(generator_);
	}

	int Random::rand_binomial(int tries, double prob) {
		return std::binomial_distribution<int>(tries, prob)(generator_);
	}

	double Random::rand_double() {
		return rand_double(0.0, 1.0);
	}

	double Random::rand_double(double range) {
		return rand_double(0.0, range);
	}

	double Random::rand_double(double lo, double hi) {
		return std::uniform_real_distribution<double>(lo, hi)(generator_);
	}

	double Random::rand_normal() {
		return rand_normal(0.0, 1.0);
	}

	double Random::rand_normal(double mean, double stddev) {
		return std::normal_distribution<double>(mean, stddev)(generator_);
	}
}

