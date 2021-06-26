#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <cstdlib>
#include <vecmath.h>
#include <float.h>
#include <cmath>
#include "math.hpp"


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction,
           const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->horizontal.normalize();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual Ray randGenRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// DONE: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
                      const Vector3f &up, int imgW, int imgH, float angle) :
        Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        this->angle = angle;
    }

    Ray generateRay(const Vector2f &point) override {
        // remember to normalize the ray's direc!
        Vector2f imgc(width / 2, height / 2);
        float dis = height / 2 / tan(angle / 2);
        
        Vector3f direc_cam((point - imgc)[0], (imgc - point)[1], dis);
        direc_cam.normalize();
        
        Matrix3f R(horizontal, -up, direction);
        Vector3f direc = R * direc_cam;

        return Ray(center, direc);
    }
    Ray randGenRay(const Vector2f &point) override {
        Vector2f sample(point.x() + randDouble(0, 1),
                        point.y() + randDouble(0, 1));
        return generateRay(sample);
    }
private:
    float angle;
};

#endif //CAMERA_H
