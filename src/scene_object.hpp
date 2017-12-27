#ifndef _SCENE_OBJECT_HPP
#define _SCENE_OBJECT_HPP

#include <vector>

#include "box.hpp"
#include "color.hpp"
#include "geometry.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

struct Intersection;
struct SceneObjectIntersection;

class SceneObject : public Geometry {
public:
    SceneObject(const Vector3D& pos, const Material& mat);
    virtual ~SceneObject();

    virtual SceneObjectIntersection Intersects(const Ray3D& ray,
                                               double max_dist = INFINITY) const = 0;
    virtual Vector3D NormalAtPoint(const Vector3D& v) const = 0;

    virtual Material GetMaterial() const;

    virtual Box GetBoundingBox() const = 0;

protected:
    Material mat;
};

#endif
