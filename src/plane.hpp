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

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;

    virtual inline Vector3D NormalAtPoint(const Vector3D& v) const override
    {
        return this->normal;
    }

protected:
    Vector3D normal;
};

#endif
