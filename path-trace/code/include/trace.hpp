#ifndef TRACE_H
#define TRACE_H

#include "Vector3f.h"
#include "material.hpp"
#include "group.hpp"
#include "math.hpp"

namespace trace{

    Vector3f randUnitSphere() {
        double theta = 2 * PI * rand() / RAND_MAX,
            rho = PI * rand() / RAND_MAX;
        return Vector3f(cos(rho) * cos(theta),
                        cos(rho) * sin(theta),
                        sin(rho));
    }

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
                      Vector3f factor) {
        if (factor.length() < 0.001) return Vector3f::ZERO;
        Hit h;
        if (group->intersect(r, h, 1e-6)) {
            Material* m = h.getMaterial();
            const Vector3f& p = r.pointAtParameter(h.getT());
            const Vector3f& n = h.getNormal();
            if (m->getEmitColor().length() > 0.001) {// light source
                
                return m->getEmitColor();
                
            } else if (fabs(m->getRefract() - 1) > 0.001) {// glass

                double it = h.isOutside() ?
                    (1.0 / m->getRefract()) : m->getRefract();

                float cos_theta = Vector3f::dot(-r.getDirection(), n);
                float sin_theta = sqrt(1 - cos_theta * cos_theta);
                bool cannot_refract = it * sin_theta > 1.0;
                /*
                if (cannot_refract ||
                    reflectance(cos_theta, it) > (double) rand() / RAND_MAX) {
                    return 0.5 *
                        getColor(reflect(r, p, n, m->getShininess()),
                                 group, bgcolor, factor * 0.5);
                                 } else*/
                    return m->getRefractColor() *
                        getColor(refract(r, p, n, m->getRefract(), h.isOutside()),
                                 group, bgcolor, factor * m->getRefractColor());

            } else {// solid

                Vector3f color = Vector3f::ZERO;
                if (m->getDiffuseColor().length() > 0.001)
                    color += m->getDiffuseColor() *
                        getColor(diffuse(p, n),
                                 group, bgcolor, factor * m->getDiffuseColor());
                if (m->getSpecularColor().length() > 0.001)
                    m->getSpecularColor() *
                        getColor(reflect(r, p, n, m->getShininess()),
                                 group, bgcolor, factor * m->getSpecularColor());
                return color;
            }
        } else {
            return bgcolor;
        }
    }
}
#endif
