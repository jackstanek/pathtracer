#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "scene_object.hpp"
#include "vector.hpp"

class Sphere : public SceneObject {
public:
    Sphere(const Vector3D& pos, double radius, const Material& mat);
    virtual ~Sphere();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;

private:
    double radius;

    virtual inline Vector3D NormalAtPoint(const Vector3D& v) const override
    {
        return this->pos.To(v).Normalized();
    };
};

#endif
