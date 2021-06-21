#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>
#include <algorithm>

#include "Vector3f.h"
#include "ray.hpp"
#include "hit.hpp"
#include "math.hpp"
#include <iostream>


// DONE: Implement Shade function that computes Phong introduced in class.
class Material {
private:
public:

    explicit Material(const Vector3f &d_color,
                      const Vector3f &s_color = Vector3f::ZERO,
                      float s = 0,
                      float r = 0,
                      const Vector3f &e_color = Vector3f::ZERO,
                      const Vector3f &r_color = Vector3f::ZERO) :
        diffuseColor(d_color), specularColor(s_color),
        shininess(s), refract(r), emitColor(e_color),
        refractColor(r_color) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }
    virtual Vector3f getSpecularColor() const {
        return specularColor;
    }
    virtual float getShininess() const {
        return shininess;
    }
    virtual float getRefract() const {
        return refract;
    }
    virtual Vector3f getRefractColor() const {
        return refractColor;
    }

    virtual Vector3f getEmitColor() const {
        return emitColor;
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
    float refract;
    Vector3f emitColor;
    Vector3f refractColor;
};


#endif // MATERIAL_H
