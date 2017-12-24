#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include "intersection.hpp"
#include "plane.hpp"
#include "vector.hpp"

class Triangle : public Plane {
public:
    Triangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3,
             const Material& mat);
    virtual ~Triangle();

    virtual SceneObjectIntersection Intersects(const Ray3D& ray, double max_dist) const override;

protected:
    Vector3D verts[3];
};

#endif
