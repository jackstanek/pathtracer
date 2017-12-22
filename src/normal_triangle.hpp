#ifndef NORMAL_TRIANGLE_HPP_
#define NORMAL_TRIANGLE_HPP_

#include "intersection.hpp"
#include "triangle.hpp"
#include "vector.hpp"

class NormalTriangle : public Triangle {
public:
    NormalTriangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3,
                   const Vector3D& norm1, const Vector3D& norm2, const Vector3D& norm3,
                   const Material& mat);

    NormalTriangle(const Vector3D* verts_, const Vector3D* norms_, const Material& mat);

    virtual ~NormalTriangle();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;
    virtual Vector3D NormalAtPoint(const Vector3D& v) const override;

protected:
    Vector3D norms[3];
};

#endif
