#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// DONE: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal;
        this->d = d;
        this->material = m;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float t = (Vector3f::dot(normal, r.getOrigin()) + d) /
            Vector3f::dot(normal, r.getDirection());

        if (t <= 0) return false;

        if (fabs(t) < tmin) return false;// impossible t
        if (fabs(t) > h.getT()) return false;// farther t

        h.set(t, material, normal);
        return true;
    }

protected:
    Vector3f normal;
    float d;
    Material* m;
};

#endif //PLANE_H
		

