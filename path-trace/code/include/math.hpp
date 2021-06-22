#ifndef MATH_H
#define MATH_H
#include <cmath>

const double PI = 3.1415926535;
const int ATALS_SAMPLE = 3;

inline float clamp(float x, float upper = 1e10) {
    if (x > upper) x = upper;
    return x > 0 ? x : 0;
}

#endif
