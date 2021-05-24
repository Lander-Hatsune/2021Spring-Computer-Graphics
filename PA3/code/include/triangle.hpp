#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;

// DONE (PA2): Copy from PA1
class Triangle: public Object3D
{

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        this->vertices[0] = a; vertices[1] = b; vertices[2] = c;
        this->material = m;
        this->normal = Vector3f::cross(b - a, c - a);
        this->normal.normalize();
        // calculate plane's d
        d = -Vector3f::dot(normal, a);
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        Vector3f S = vertices[0] - ray.getOrigin(),
            E1 = vertices[0] - vertices[1],
            E2 = vertices[0] - vertices[2];

        Vector3f ans(Matrix3f(S, E1, E2).determinant(),
                     Matrix3f(ray.getDirection(), S, E2).determinant(),
                     Matrix3f(ray.getDirection(), E1, S).determinant());
        ans = ans / Matrix3f(ray.getDirection(), E1, E2).determinant();
        if (ans[0] < 0) return false;
        if (ans[1] < 0 || ans[1] > 1 || ans[2] < 0 || ans[2] > 1) return false;
        if (ans[1] + ans[2] > 1) return false;
        if (ans[0] < tmin) return false;
        if (ans[0] > hit.getT()) return false;

        hit.set(ans[0], material, normal);
        return true;
	}
	Vector3f normal;
    float d;
	Vector3f vertices[3];

    void drawGL() override {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

protected:
};

#endif //TRIANGLE_H
