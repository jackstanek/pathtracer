#ifndef RECT_PRISM_HPP_
#define RECT_PRISM_HPP_

#include <cmath>

#include "geometry.hpp"
#include "ray.hpp"
#include "vector.hpp"

struct Intersection;

enum BoxExtent {
    BE_MIN_EXTENT = 0,
    BE_MAX_EXTENT,
    N_EXTENTS
};

/* This is an axis-aligned box.
 *
 * TODO: Make some sort of arbitrarily-aligned box class.
 */
class Box : public Geometry {
public:
    /* Construct an axis-aligned bounding box with the given
       extents */
    Box(const Vector3D& min_extent = Vector3D(),
        const Vector3D& max_extent = Vector3D());

    /* Construct a bounding box around two boxes */
    Box(const Box& left, const Box& right);

    virtual ~Box();

    virtual Intersection Intersects(const Ray3D& ray,
                                    double max_dist = INFINITY) const;

    void Expand(const Box& box);
    int LongestAxis() const;

    /* b is engulfed by this box if it is contained completely within
       it. */
    bool Engulfs (const Box& b) const;

private:
    Vector3D extents[N_EXTENTS];
};

#endif
