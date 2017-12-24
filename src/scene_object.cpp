#include <algorithm>
#include <cmath>

#include "color.hpp"
#include "geometry.hpp"
#include "intersection.hpp"
#include "normal_triangle.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "vector.hpp"

SceneObject::SceneObject(const Vector3D& pos, const Material& mat) :
    Geometry(pos),
    mat(mat)
{
}

SceneObject::~SceneObject()
{
}

Material SceneObject::GetMaterial() const
{
    return this->mat;
}
