#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include <stdint.h>
#include <vector>

#include "color.hpp"
#include "scene_parser.hpp"

struct Material {
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular, const Color& transmissivity,
             double phong_exp, double ior) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        transmissivity(transmissivity),
        phong_exp(phong_exp),
        ior(ior)
    {
    }
    Material(const SceneComponent* sc);

    Color ambient;
    Color diffuse;
    Color specular;
    Color transmissivity;

    double phong_exp, ior;
};

typedef std::vector<Material> MaterialPool;
extern const Material DEFAULT_MAT;

#endif
