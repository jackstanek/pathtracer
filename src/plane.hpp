#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "intersection.hpp"
#include "ray.hpp"
#include "scene_object.hpp"
#include "vector.hpp"

class Plane : public SceneObject {
public:
    Plane(const Vector3D& origin, const Vector3D& normal, const Material& mat);
    virtual ~Plane();

    virtual SceneObjectIntersection Intersects(const Ray3D& ray,
                                               double max_dist) const override;

    virtual Vector3D NormalAtPoint(const Vector3D& v) const override;

protected:
    Vector3D normal;
};

#endif
