#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <cassert>

// DONE: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3f::ZERO;
        radius = 1.;
        material = NULL;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
        this->material = material;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f l = center - r.getOrigin();

        float t_p = Vector3f::dot(l, r.getDirection());

        t_p = (l.length() > radius) ? t_p : -t_p;// inside : outside

        if (t_p < 0) return false;// wrong angle

        float d_square = l.squaredLength() - t_p * t_p;

        if (d_square >= radius * radius) return false;// wrong distance

        float t = t_p - sqrt(radius * radius - d_square);

        bool outside = true;
        if (t < 0) {outside = false; t = -t;}
        
        if (t < tmin) return false;// impossible t
        if (t > h.getT()) return false;// farther t

        Vector3f n(r.pointAtParameter(t) - center);
        n.normalize();
        float theta = acos(-n.y());
        float phi = atan2(-n.z(), n.x()) + PI;
        h.set(t, material, n, theta / PI, phi / 2 / PI, outside);
        return true;
    }

protected:
    Vector3f center;
    float radius;
    Material* material;
};


#endif
