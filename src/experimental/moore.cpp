#include <iostream>
//#include <array>
#include <vector>

#include "Coord.hpp"

#ifndef DIM
#define DIM 3
#endif

int64_t pow(int64_t base, int64_t exp) {
    int64_t result = 1;
    while (exp != 0) {
        if ((exp & 1) != 0) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

int64_t moore_count(int64_t dims, int64_t dist) {
    return pow(2 * dist + 1, dims) - 1;
}

template<std::size_t N>
using Move = mg_util::Coord<N>;
// using Move = std::array<int32_t, N>;

template<std::size_t N>
using Moves = std::vector<Move<N>>;

template<std::size_t N>
static void moore_helper(int32_t dist, Moves<N> & ps, int32_t dim, Move<N> & cur, bool valid) {
    if (dim >= N) {
        if (valid) {
            ps.push_back(cur);
        }
    } else {
        for (int32_t i = -dist; i <= dist; i++) {
            cur[dim] = i;
            moore_helper(dist, ps, dim + 1, cur, valid || i != 0);
        }
    }
}

template<std::size_t N>
auto moore(int32_t dist) {
    Moves<N> ret;
    ret.reserve(moore_count(N, dist));
    Move<N> cur;
    moore_helper<N>(dist, ret, 0, cur, false);
    return ret;
}

int main(int argc, char ** argv) {
    int32_t dist = argc > 1 ? std::atoi(argv[1]) : 1;
    auto n = moore<DIM>(dist);
    std::cout << moore_count(DIM, dist) << '\n';
    std::cout << n.size() << '\n';
    /*
    for (const auto & p : n) {
        for (const auto & d : p) {
            std::cout << d << ' ';
        }
        std::cout << '\n';
    }
    */
}
