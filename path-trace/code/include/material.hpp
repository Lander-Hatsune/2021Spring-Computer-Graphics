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
                      const Vector3f &r_color = Vector3f::ZERO,
                      char const *d_filename = nullptr,
                      char const *s_filename = nullptr,
                      char const *e_filename = nullptr,
                      int width = 0, int height = 0, float density = 1.);

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor(float x, float y) const {
        if (d_data) {
            x = clamp(x, 1); y = clamp(y, 1);
            int i = (x * width), j = (y * height);
            if (i >= width) i = width - 1;
            if (j >= height) j = height - 1;
            auto pixel = d_data + j * 3 * width + i * 3;
            return Vector3f((double)pixel[0] / 255,
                            (double)pixel[1] / 255,
                            (double)pixel[2] / 255);
        } else return diffuseColor;
    }
    virtual Vector3f getSpecularColor(float x, float y) const {
        if (s_data) {
            x = clamp(x, 1); y = clamp(y, 1);
            int i = (x * width), j = (y * height);
            if (i >= width) i = width - 1;
            if (j >= height) j = height - 1;
            auto pixel = s_data + j * 3 * width + i * 3;
            return Vector3f((double)pixel[0] / 255,
                            (double)pixel[1] / 255,
                            (double)pixel[2] / 255);
        } else return specularColor;
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
    virtual float getDensity() const {
        return density;
    }

    virtual Vector3f getEmitColor(float x, float y) const {
        if (e_data) {
            x = clamp(x, 1); y = clamp(y, 1);
            int i = (x * width), j = (y * height);
            if (i >= width) i = width - 1;
            if (j >= height) j = height - 1;
            auto pixel = e_data + j * 3 * width + i * 3;
            return Vector3f((double)pixel[0] / 255,
                            (double)pixel[1] / 255,
                            (double)pixel[2] / 255);
        }
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
    unsigned char* d_data;
    unsigned char* s_data;
    unsigned char* e_data;
    int width, height;
    float density;
};

#endif // MATERIAL_H
