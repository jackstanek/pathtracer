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
    assert(min_extent < max_extent);
}

Box::Box(const Box& left, const Box& right) :
    Geometry(((left.pos - left.extents[BE_MIN_EXTENT]) +
              (right.pos + right.extents[BE_MAX_EXTENT])) / 2)
{
    auto left_min = left.extents[BE_MIN_EXTENT], right_min = right.extents[BE_MIN_EXTENT];
    extents[BE_MIN_EXTENT] = Vector3D(std::min(left_min.GetX(), right_min.GetX()),
                                      std::min(left_min.GetY(), right_min.GetY()),
                                      std::min(left_min.GetZ(), right_min.GetZ()));
    auto left_max = left.extents[BE_MAX_EXTENT], right_max = right.extents[BE_MAX_EXTENT];
    extents[BE_MAX_EXTENT] = Vector3D(std::max(left_max.GetX(), right_max.GetX()),
                                      std::max(left_max.GetY(), right_max.GetY()),
                                      std::max(left_max.GetZ(), right_max.GetZ()));

    assert(left <= *this);
    assert(right <= *this);
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

    Vector3D pt = ray.Point(tmin);
    return Intersection(nullptr,
                        true,
                        ray,
                        pt);
}

bool Box::operator<= (const Box& box) const
{
    return box.extents[BE_MIN_EXTENT] <= extents[BE_MIN_EXTENT]
        && extents[BE_MAX_EXTENT] <= extents[BE_MAX_EXTENT];

}
