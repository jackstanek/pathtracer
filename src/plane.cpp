#include "intersection.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "scene_object.hpp"
#include "vector.hpp"

Plane::Plane(const Vector3D& pos, const Vector3D& normal, const Material& mat) :
    SceneObject(pos, mat),
    normal(normal.Normalized())
{
}

Plane::~Plane()
{
}

SceneObjectIntersection Plane::Intersects(const Ray3D &ray, double max_dist) const
{
    double denom = ray.GetDir().Dot(this->normal);

    /* Infinite / zero intersections if ray perpendicular to normal */
    if (is_zero(denom)) {
        return SceneObjectIntersection(this, false, ray);
    } else {
        double t = (this->pos - ray.GetOrigin()).Dot(this->normal) / denom;

        if (t >= EPSILON && t <= max_dist) {
            return SceneObjectIntersection(this,
                                           true,
                                           ray,
                                           INC_INWARD,
                                           ray.Point(t),
                                           this->normal);
        } else {
            return SceneObjectIntersection(this, false, ray);
        }
    }
}

Vector3D Plane::NormalAtPoint(const Vector3D &v) const
{
    return this->normal;
}
