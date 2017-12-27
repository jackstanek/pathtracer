#include <algorithm>
#include <cassert>

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
    assert(min_extent <= max_extent);
}

Box::Box(const Box& left, const Box& right) :
    Geometry(((left.pos - left.extents[BE_MIN_EXTENT]) +
              (right.pos + right.extents[BE_MAX_EXTENT])) / 2),
    extents {Vector3D(), Vector3D()}
{
    Expand(left);
    Expand(right);
}

Box::~Box()
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

    if (tmin > max_dist) {
        return Intersection(nullptr, false, ray);
    }

    Vector3D pt = ray.Point(tmin);
    return Intersection(nullptr,
                        true,
                        ray,
                        pt);
}

void Box::Expand(const Box &box)
{
    /* If this is a zero-volume box, expand to engulf box. */
    if (extents[BE_MIN_EXTENT] == extents[BE_MAX_EXTENT]) {
        extents[BE_MIN_EXTENT] = box.extents[BE_MIN_EXTENT];
        extents[BE_MAX_EXTENT] = box.extents[BE_MAX_EXTENT];
    } else {
        extents[BE_MIN_EXTENT] =
            Vector3D::MinCombination(extents[BE_MIN_EXTENT],
                                     box.extents[BE_MIN_EXTENT]);
        extents[BE_MAX_EXTENT] =
            Vector3D::MaxCombination(extents[BE_MAX_EXTENT],
                                     box.extents[BE_MAX_EXTENT]);
    }

    assert(Engulfs(box));
}

int Box::LongestAxis() const
{
    return extents[BE_MIN_EXTENT]
        .To(extents[BE_MAX_EXTENT])
        .LongestAxis();
}

bool Box::Engulfs (const Box& box) const
{
    return extents[BE_MIN_EXTENT] <= box.extents[BE_MIN_EXTENT]
        && box.extents[BE_MAX_EXTENT] <= extents[BE_MAX_EXTENT];
}
