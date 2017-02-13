#pragma once

#include "mg_util_include.hpp"

namespace mg_util {
// returns the power of two that is at least x
int64_t pow2roundup(int64_t x);
// returns the power of two that is at most x
int64_t pow2rounddown(int64_t x);

// clamp x so that it is at least lo and at most hi
int64_t clamp_inc(int64_t x, int64_t lo, int64_t hi);
// clamp x so that it is at least lo and at most hi - 1
int64_t clamp_exc(int64_t x, int64_t lo, int64_t hi);
}

