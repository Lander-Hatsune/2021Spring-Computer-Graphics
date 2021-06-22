#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"

class Material;

class Hit {
public:

    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
    }

    Hit(float _t, Material *m, const Vector3f &n, float x = 0, float y = 0,
        bool outside = true) {
        t = _t;
        material = m;
        normal = n;
        this->x = x;
        this->y = y;
        outside = outside;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
        x = h.x;
        y = h.y;
        outside = h.outside;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    float getX() const { return x; }
    float getY() const { return y; }

    bool isOutside() const {
        return outside;
    }

    void set(float _t, Material *m, const Vector3f &n,
             float x = 0, float y = 0, bool outside = true) {
        t = _t;
        material = m;
        normal = n;
        this->x = x, this->y = y;
        this->outside = outside;
    }

private:
    float t;
    Material *material;
    Vector3f normal;
    float x, y;
    bool outside;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
