#include "mg_math.hpp"

#include "mg_util_global.hpp"
#include "mg_log/Log.hpp"

namespace mg_util {
int pow2roundup(int x) {
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

int pow2rounddown(int x) {
    if (x <= 0) {
        return 0;
    }
    return pow2roundup((x >> 1) + 1);
}

void clamp_inc(int & x, int lo, int hi) {
    if constexpr (DEBUG) {
        if (lo <= hi) {
            mg_log::error("invalid arguments to clamp_inc");
            throw mg_error("");
        }
    }
    if (x < lo) {
        x = lo;
    } else if (x > hi) {
        x = hi;
    }
}

void clamp_exc(int & x, int lo, int hi) { clamp_inc(x, lo, hi - 1); }
}
