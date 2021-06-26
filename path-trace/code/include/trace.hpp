#ifndef TRACE_H
#define TRACE_H

#include "Vector3f.h"
#include "material.hpp"
#include "group.hpp"
#include "math.hpp"

namespace trace{

    Ray diffuse(const Vector3f& p, const Vector3f& n) {
        Vector3f ori = p + n;
        return Ray(p, (ori + randUnitSphere() - p).normalized());
    }
    
    Ray reflect(const Ray& r, const Vector3f& p, const Vector3f& n, const float s) {
        Vector3f direc = r.getDirection() - n * 
            2 * Vector3f::dot(r.getDirection(), n);
        return Ray(p, (direc + 1 / s * randUnitSphere()).normalized());
    }

    Ray refract(const Ray &r, const Vector3f& p, const Vector3f& n,
                const float f, bool outside) {
        double itr = outside ? f : (1.0 / f);

        float cos_theta = Vector3f::dot(-r.getDirection(), n);
        float sin_theta = sqrt(1 - cos_theta * cos_theta);

        Vector3f out_direc_perp = 1 / itr * (r.getDirection() +
                                             Vector3f::dot(-r.getDirection(),
                                                           n) * n);
        Vector3f out_direc_parl = -sqrt(1 - out_direc_perp.squaredLength()) * n;
        return Ray(p, (out_direc_perp + out_direc_parl).normalized());
    }

    float reflectance(float cosine, float ref_idx) {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }

    Vector3f getColor(const Ray& r, Group* group, Vector3f bgcolor,
                      Vector3f factor, int depth) {
        if (depth > MAX_DEPTH || factor.length() < 0.001) return Vector3f::ZERO;
        Hit h;
        if (group->intersect(r, h, 1e-6)) {
            Material* m = h.getMaterial();
            const Vector3f& p = r.pointAtParameter(h.getT());
            const Vector3f& n = h.getNormal();
            if (m->getEmitColor(h.getX(), h.getY()).length() > 0.001) {// light source
                return m->getEmitColor(h.getX(), h.getY());
                
            } else if (m->getRefract() - 1 > 0.001) {// glass

                double it = h.isOutside() ?
                    (1.0 / m->getRefract()) : m->getRefract();

                float cos_theta = Vector3f::dot(-r.getDirection(), n);
                float sin_theta = sqrt(1 - cos_theta * cos_theta);
                bool cannot_refract = it * sin_theta > 1.0;

                if (cannot_refract ||
                    reflectance(cos_theta, it) > (double) rand() / RAND_MAX) {
                    return 0.9 *
                        getColor(reflect(r, p, n, m->getShininess()),
                                 group, bgcolor, factor * 0.9, depth + 1);
                } else
                    return m->getRefractColor() *
                        getColor(refract(r, p, n, m->getRefract(), h.isOutside()),
                                 group, bgcolor, factor * m->getRefractColor(),
                                 depth + 1);

            } else {// solid

                Vector3f color = Vector3f::ZERO;
                if (m->getSpecularColor(h.getX(), h.getY()).length() > 0.001) {
                    
                    color += m->getSpecularColor(h.getX(), h.getY()) *
                        getColor(reflect(r, p, n, m->getShininess()),
                                 group, bgcolor,
                                 factor * m->getSpecularColor(h.getX(), h.getY()),
                                 depth + 1);
                    if (m->getSpecularColor(h.getX(), h.getY()).length() > 1.5) {
                        return color;
                    }
                }
                if (m->getDiffuseColor(h.getX(), h.getY()).length() > 0.001)
                    color += m->getDiffuseColor(h.getX(), h.getY()) *
                        getColor(diffuse(p, n),
                                 group, bgcolor,
                                 factor * m->getDiffuseColor(h.getX(), h.getY()),
                                 depth + 1);
                return color;
            }
        } else {
            return bgcolor;
        }
    }
}
#endif
