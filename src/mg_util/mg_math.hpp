//
// Created by Arthur Eubanks on 24/12/15.
//

#ifndef MAPGEN_MATH_H
#define MAPGEN_MATH_H

namespace mg_util {
// returns the power of two that is at least x
int pow2roundup(int x);
// returns the power of two that is at most x
int pow2rounddown(int x);

// clamp x so that it is at least lo and at most hi
void clamp_inc(int & x, int lo, int hi);
// clamp x so that it is at least lo and at most hi - 1
void clamp_exc(int & x, int lo, int hi);
}

#endif // MAPGEN_MATH_H
