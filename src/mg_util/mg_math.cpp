#include "mg_math.hpp"

#include "mg_log/Log.hpp"

namespace mg_util {
int64_t pow2roundup(int64_t x) {
    if (x < 0) {
        return 0;
    }
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

int64_t pow2rounddown(int64_t x) {
    if (x <= 0) {
        return 0;
    }
    return pow2roundup((x >> 1) + 1);
}

int64_t clamp_inc(int64_t x, int64_t lo, int64_t hi) {
    if constexpr (DEBUG) {
        if (lo <= hi) {
            mg_log::error("invalid arguments to clamp_inc");
            throw mg_error("");
        }
    }
    if (x < lo) {
        return lo;
    } else if (x > hi) {
        return hi;
    }
    return x;
}

int64_t clamp_exc(int64_t x, int64_t lo, int64_t hi) {
    return clamp_inc(x, lo, hi - 1);
}
}
