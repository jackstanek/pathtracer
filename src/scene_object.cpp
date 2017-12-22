#include <algorithm>
#include <cmath>

#include "color.hpp"
#include "intersection.hpp"
#include "normal_triangle.hpp"
#include "plane.hpp"
#include "scene_object.hpp"
#include "triangle.hpp"
#include "vector.hpp"

Intersection::Intersection(const SceneObject* obj,
                           bool intersected,
                           const Ray3D& along,
                           int inc,
                           const Vector3D& point,
                           const Vector3D& norm):
    obj(obj),
    intersected(intersected),
    inc(inc),
    dist((point - along.GetOrigin()).Norm()),
    point(point),
    norm(point, norm)
{
}

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
