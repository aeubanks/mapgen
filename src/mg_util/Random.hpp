//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_RANDOM_H
#define MAP_GEN_RANDOM_H


#include <random>

namespace mg_util {
	class Random {
	public:
		Random();
		Random(int seed);

		// returns true/false with equal probability
		bool rand_bool();
		// returns true with probability chance, false with probability (1.0 - chance)
		bool rand_bool(double chance);
		// returns a random int with equality probability in the range (0 - range)
		int rand_int_inc(int range);
		// returns a random int with equality probability in the range (lo - hi)
		int rand_int_inc(int lo, int hi);
		// returns a random int with equality probability in the range (0 - (range - 1))
		int rand_int_exc(int range);
		// returns a random int with equality probability in the range (lo - (hi - 1))
		int rand_int_exc(int lo, int hi);
		// returns a binomial distribution with # tries, probability of success of prob
		int rand_binomial(int tries, double prob);
		// returns a random double in the range (0.0 - 1.0)
		double rand_double();
		// returns a random double in the range (0.0 - range)
		double rand_double(double range);
		// returns a random double in the range (lo - hi)
		double rand_double(double lo, double hi);
		// returns a random normal distribution with mean 0.0, stddev 1.0
		double rand_normal();
		// returns a random normal distribution with the specified mean and stddev
		double rand_normal(double mean, double stddev);

	private:
		std::mt19937 generator_;
	};
}


#endif //MAP_GEN_RANDOM_H
