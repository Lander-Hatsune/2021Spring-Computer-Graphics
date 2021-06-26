#include "material.hpp"
#include "vecmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Material::Material(const Vector3f &d_color,
                   const Vector3f &s_color,
                   float s,
                   float r,
                   const Vector3f &e_color,
                   const Vector3f &r_color,
                   char const *d_filename,
                   char const *s_filename,
                   char const *e_filename,
                   int width, int height, float d) :
    diffuseColor(d_color), specularColor(s_color),
    shininess(s), refract(r), emitColor(e_color),
    refractColor(r_color),
    width(width), height(height), density(d) {
    int channels = 3;
    d_data = nullptr, s_data = nullptr;
    if (d_filename)
        d_data = stbi_load(d_filename, &width, &height, &channels, channels);
    else d_data = nullptr;
    if (s_filename)
        s_data = stbi_load(s_filename, &width, &height, &channels, channels);
    else s_data = nullptr;
    if (e_filename)
        e_data = stbi_load(e_filename, &width, &height, &channels, channels);
    else e_data = nullptr;
}
