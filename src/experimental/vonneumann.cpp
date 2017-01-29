#include <iostream>
//#include <array>
#include <vector>

#include "Coord.hpp"

#ifndef DIM
#define DIM 3
#endif

uint64_t n_choose_k(uint64_t n, uint64_t k) {
    if (k > n) {
        return 0;
    }
    if (k * 2 > n) {
        k = n - k;
    }
    if (k == 0) {
        return 1;
    }
    uint64_t result = n;
    for (uint64_t i = 2; i <= k; ++i) {
        result *= n - i + 1;
        result /= i;
    }
    return result;
}

uint64_t delannoy(uint64_t m, uint64_t n) {
    uint64_t result = 0;
    uint64_t upper = std::min(m, n);
    for (uint64_t k = 1; k <= upper; ++k) {
        result += n_choose_k(n, k) * n_choose_k(m, k) * (1ULL << k);
    }
    return result;
}

uint64_t vonneumann_count(uint64_t dims, uint64_t dist) {
    return delannoy(dims, dist);
}

template<std::size_t N>
using Move = mg_util::Coord<N>;
//using Move = std::array<int, N>;

template<std::size_t N>
using Moves = std::vector<Move<N>>;

template<std::size_t N>
static void vonneumann_helper(int dist, Moves<N> & ps, int dim, Move<N> & cur, bool valid) {
    if (dim >= N) {
        if (valid) {
            ps.push_back(cur);
        }
    } else {
        for (int i = 0; i <= dist; i++) {
            cur[dim] = i;
            vonneumann_helper(dist - i, ps, dim + 1, cur, valid || i != 0);
            if (i != 0) {
                cur[dim] = -i;
                vonneumann_helper(dist - i, ps, dim + 1, cur, valid || i != 0);
            }
        }
    }
}

template<std::size_t N>
auto vonneumann(int dist) {
    Moves<N> ret;
    ret.reserve(vonneumann_count(N, dist));
    Move<N> cur;
    vonneumann_helper<N>(dist, ret, 0, cur, false);
    return ret;
}

int main(int argc, char ** argv) {
    int dist = argc > 1 ? std::atoi(argv[1]) : 1;
    std::cout << vonneumann_count(DIM, dist) << '\n';
    auto n = vonneumann<DIM>(dist);
    
    /*
    for (const auto & p : n) {
        for (const auto & d : p) {
            std::cout << d << ' ';
        }
        std::cout << '\n';
    }
    */
    
    std::cout << n.size() << '\n';
}
