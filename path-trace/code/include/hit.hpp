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

    Hit(float _t, Material *m, const Vector3f &n, bool outside = true) {
        t = _t;
        material = m;
        normal = n;
        outside = outside;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
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

    bool isOutside() const {
        return outside;
    }

    void set(float _t, Material *m, const Vector3f &n, bool outside = true) {
        t = _t;
        material = m;
        normal = n;
        this->outside = outside;
    }

private:
    float t;
    Material *material;
    Vector3f normal;
    bool outside;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
