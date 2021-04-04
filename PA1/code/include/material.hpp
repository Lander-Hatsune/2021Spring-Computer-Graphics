#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>
#include <algorithm>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

inline float clamp(float x) {
    return x > 0 ? x : 0;
}

// DONE: Implement Shade function that computes Phong introduced in class.
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        shaded = lightColor *
            (diffuseColor * clamp(Vector3f::dot(dirToLight, hit.getNormal()))
             + specularColor * pow(clamp(Vector3f::dot(-ray.getDirection(),
                                                       2 * Vector3f::dot(hit.getNormal(), dirToLight)
                                                       * hit.getNormal() - dirToLight)), shininess));
        return shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
