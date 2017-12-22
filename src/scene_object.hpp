#ifndef _SCENE_OBJECT_HPP
#define _SCENE_OBJECT_HPP

#include <vector>

#include "color.hpp"
#include "light.hpp"
#include "material.hpp"
#include "scene_object.hpp"
#include "ray.hpp"
#include "vector.hpp"

struct Intersection;

class SceneObject {
public:
    SceneObject(const Vector3D& pos, const Material& mat);
    virtual ~SceneObject();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist = INFINITY) const = 0;
    virtual Vector3D NormalAtPoint(const Vector3D& v) const = 0;

    virtual Material GetMaterial() const;

protected:
    Vector3D pos;
    Material mat;
};

#endif
