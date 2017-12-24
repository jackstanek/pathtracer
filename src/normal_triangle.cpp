#include "normal_triangle.hpp"

NormalTriangle::NormalTriangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3,
                               const Vector3D& norm1, const Vector3D& norm2, const Vector3D& norm3,
                               const Material& mat) :
    Triangle(vert1, vert2, vert3, mat)
{
    this->norms[0] = norm1;
    this->norms[1] = norm2;
    this->norms[2] = norm3;
}

NormalTriangle::NormalTriangle(const Vector3D* verts_, const Vector3D* norms_, const Material& mat) :
    Triangle(verts_[0], verts_[1], verts_[2], mat)
{
    for (int i = 0; i < 3; i++) {
        this->norms[i] = norms_[i];
    }
}

NormalTriangle::~NormalTriangle()
{
}

SceneObjectIntersection NormalTriangle::Intersects(const Ray3D &ray, double max_dist) const
{
    SceneObjectIntersection intersected = Triangle::Intersects(ray, max_dist);
    if (intersected.intersected) {
        intersected.norm = Ray3D(intersected.point,
                                 this->NormalAtPoint(intersected.point));
    }

    return intersected;
}

Vector3D NormalTriangle::NormalAtPoint(const Vector3D &v) const
{
    Vector3D acc;
    double area = this->verts[0].To(this->verts[1]).Cross(this->verts[0].To(this->verts[2])).Norm();

    for (int i = 0; i < 3; i++) {
        Vector3D v1 = this->verts[(i + 1) % 3].To(this->verts[(i + 2) % 3]),
            v2 = this->verts[(i + 1) % 3].To(v);
        acc += this->norms[i] * (v1.Cross(v2).Norm() / area);
    }

    return acc;
}
