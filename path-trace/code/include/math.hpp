#ifndef MATH_H
#define MATH_H
#include <cmath>
#include "vecmath.h"

const double PI = 3.1415926535;
const int ATALS_SAMPLE = 10;
const int MAX_DEPTH = 10;

inline double randDouble(double m, double M) {
    return ((double)rand() / RAND_MAX) * (M - m) + m;
}

inline Vector3f randUnitSphere() {
    Vector3f ret(1, 1, 1);
    do {
        float x = randDouble(-1, 1),
            y = randDouble(-1, 1),
            z = randDouble(-1, 1);
        ret = Vector3f(x, y, z);
    } while (ret.squaredLength() > 1);
    return ret.normalized();
}

inline float clamp(float x, float upper = 1e10) {
    if (x > upper) x = upper;
    return x > 0 ? x : 0;
}

#endif
