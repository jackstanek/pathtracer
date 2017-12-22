#include <vector>

#include "material.hpp"
#include "scene_parser.hpp"

static std::vector<Material> materials;

Material::Material(const SceneComponent* sc)
{
    ValueList v = sc->values();
    this->ambient = Color(v[0].d_val, v[1].d_val, v[2].d_val);
    this->diffuse = Color(v[3].d_val, v[4].d_val, v[5].d_val);
    this->specular = Color(v[6].d_val, v[7].d_val, v[8].d_val);
    this->transmissivity = Color(v[10].d_val, v[11].d_val, v[12].d_val);
    this->phong_exp = v[9].d_val;
    this->ior = v[13].d_val;
}

const Material DEFAULT_MAT(Color(0, 0, 0),
                           Color(1, 1, 1),
                           Color(0, 0, 0),
                           Color(0, 0, 0),
                           5, 1);
