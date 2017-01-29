#include <iostream>
//#include <array>
#include <vector>

#include "Coord.hpp"

#ifndef DIM
#define DIM 3
#endif

uint64_t pow(uint64_t base, uint64_t exp) {
    uint64_t result = 1;
    while (exp != 0) {
        if ((exp & 1) != 0) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

uint64_t moore_count(uint64_t dims, uint64_t dist) {
    return pow(2 * dist + 1, dims) - 1;
}

template<std::size_t N>
using Move = mg_util::Coord<N>;
// using Move = std::array<int, N>;

template<std::size_t N>
using Moves = std::vector<Move<N>>;

template<std::size_t N>
static void moore_helper(int dist, Moves<N> & ps, int dim, Move<N> & cur, bool valid) {
    if (dim >= N) {
        if (valid) {
            ps.push_back(cur);
        }
    } else {
        for (int i = -dist; i <= dist; i++) {
            cur[dim] = i;
            moore_helper(dist, ps, dim + 1, cur, valid || i != 0);
        }
    }
}

template<std::size_t N>
auto moore(int dist) {
    Moves<N> ret;
    ret.reserve(moore_count(N, dist));
    Move<N> cur;
    moore_helper<N>(dist, ret, 0, cur, false);
    return ret;
}

int main(int argc, char ** argv) {
    int dist = argc > 1 ? std::atoi(argv[1]) : 1;
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
