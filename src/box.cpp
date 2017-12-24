#include <algorithm>

#include "box.hpp"
#include "geometry.hpp"
#include "helper.hpp"
#include "intersection.hpp"
#include "vector.hpp"

Box::Box(const Vector3D& min_extent,
         const Vector3D& max_extent) :
    Geometry((min_extent + max_extent) / 2),
    extents {min_extent, max_extent}
{
}

Intersection Box::Intersects(const Ray3D& ray, double max_dist) const
{
    Vector3D origin = ray.GetOrigin(), inv_dir = ray.GetInvDir();
    double txmin = (this->extents[BE_MIN_EXTENT].GetX() - origin.GetX()) * inv_dir.GetX(),
        txmax = (this->extents[BE_MAX_EXTENT].GetX() - origin.GetX()) * inv_dir.GetX(),
        tymin = (this->extents[BE_MIN_EXTENT].GetY() - origin.GetY()) * inv_dir.GetY(),
        tymax = (this->extents[BE_MAX_EXTENT].GetY() - origin.GetY()) * inv_dir.GetY(),
        tzmin = (this->extents[BE_MIN_EXTENT].GetZ() - origin.GetZ()) * inv_dir.GetZ(),
        tzmax = (this->extents[BE_MAX_EXTENT].GetZ() - origin.GetZ()) * inv_dir.GetZ();

    double tmin = std::max(std::max(std::min(txmin, txmax), std::min(tymin, tymax)), std::min(tzmin, tzmax));
    double tmax = std::min(std::min(std::max(txmin, txmax), std::max(tymin, tymax)), std::max(tymin, tymax));

    if (tmax < 0) {
        return Intersection(nullptr, false, ray);
    }

    if (tmin > tmax) {
        return Intersection(nullptr, false, ray);
    }

    Vector3D pt = ray.Point(tmin);
    return Intersection(nullptr,
                        true,
                        ray,
                        pt);
}

bool Box::ValidBoundingBox() const
{
    return !is_approx(extents[BE_MIN_EXTENT].To(extents[BE_MAX_EXTENT]).Norm(), 0);
}
