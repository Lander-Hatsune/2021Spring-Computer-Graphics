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
        float t = - (Vector3f::dot(normal, r.getOrigin()) + d) /
            Vector3f::dot(normal, r.getDirection());

        if (t <= 0) return false;

        if (fabs(t) < tmin) return false;// impossible t
        if (fabs(t) > h.getT()) return false;// farther t

        Vector3f p = r.pointAtParameter(t);
        float x, y;

        if (std::fabs(p.z()) - (int)std::fabs(p.z()) < 0.001) {
            x = r.pointAtParameter(t).x(), y = r.pointAtParameter(t).y();
        } else if (std::fabs(p.y()) - (int)std::fabs(p.y()) < 0.001) {
            x = r.pointAtParameter(t).x(), y = r.pointAtParameter(t).z();
        } else if (std::fabs(p.x()) - (int)std::fabs(p.x()) < 0.001) {
            x = r.pointAtParameter(t).y(), y = r.pointAtParameter(t).z();
        }
        x /= 15, y /= 15;
        x = std::fabs(x), y = std::fabs(y);
        x -= (int) x, y -= (int) y;
        
        h.set(t, material, normal, x, y);
        return true;
    }

protected:
    Vector3f normal;
    float d;
    Material* m;
};

#endif //PLANE_H
		

