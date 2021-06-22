#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// DONE (PA3): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }


    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // DONE (PA3): fill in data vector
        std::vector<Vector3f> iterpoints;

        resolution *= this->getControls().size() - 1;

        for (int i = 0; i < resolution; i++) {
            double t = (double) i / resolution;
            for (auto p: this->getControls()) {
                iterpoints.push_back(p);
            }
            while (iterpoints.size() > 2) {
                for (int i = 0; i < iterpoints.size() - 1; i++) {
                    iterpoints[i] = t * iterpoints[i] +
                        (1 - t) * iterpoints[i + 1];
                }
                iterpoints.pop_back();
            }
            data.push_back(CurvePoint{
                    t * iterpoints[0] +
                    (1 - t) * iterpoints[1],
                    (iterpoints[1] - iterpoints[0]).normalized()});
            
            iterpoints.pop_back(); iterpoints.pop_back();
        }
    }

protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // DONE (PA3): fill in data vector
        const int k = 3;
        int n = this->getControls().size() - 1;

        resolution *= n;

        std::vector<Vector3f> control = this->getControls();

        std::vector<double> ti;
        for (int i = 0; i <= n + k + 1; i++) {
            ti.push_back((double) i / (n + k + 1));
        }

        for (int res_i = 0; res_i < resolution; res_i++) {
            double t = (ti[n + 1] - ti[k]) * ((double)res_i / resolution) + ti[k];
            std::vector<double> b;
            std::vector<double> b_;
            for (int i = 0; i < n + k + 1; i++) {
                b.push_back((ti[i] <= t && t < ti[i + 1]) ? 1 : 0);
            }
            int p = 0;
            while (p < k) {
                p += 1;

                for (int i = 0; i < b.size() - 1; i++) {
                    b[i] = (t - ti[i]) / (ti[i + p] - ti[i]) * b[i] +
                        (ti[i + p + 1] - t) / (ti[i + p + 1] - ti[i + 1]) * b[i + 1];
                }

                b.pop_back();

                if (p == k - 1) {
                    for (int i = 0; i < b.size() - 1; i++) {
                        b_.push_back(b[i] / (ti[i + k] - ti[i]) -
                                     b[i + 1] / (ti[i + k + 1] - ti[i + 1]));
                    }
                }
            }
            Vector3f V, T;
            //printf("%d, %d", b.size(), control.size());
            for (int i = 0; i < control.size(); i++) {
                V += b[i] * control[i];
                T += b_[i] * control[i];
            }
            data.push_back(CurvePoint{V, T.normalized()});
        }
    }

protected:

};

#endif // CURVE_HPP
