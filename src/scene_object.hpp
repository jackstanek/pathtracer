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

    const Box* GetBoundingBox() const;

protected:
    Material mat;

    /* A container for this scene object's bounding box. Should be
       initialized in subclasses. */
    Box bounding_box;
};

#endif
