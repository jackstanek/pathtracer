#include "intersection.hpp"
#include "ray.hpp"
#include "triangle.hpp"
#include "vector.hpp"

Triangle::Triangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3, const Material& mat) :
    Plane(vert1,
          vert1.To(vert3).Cross(vert1.To(vert2)),
          mat)
{
    this->verts[0] = vert1;
    this->verts[1] = vert2;
    this->verts[2] = vert3;
}

Triangle::~Triangle()
{
}

/* Möller-Trumbore algorithm implementation from
   https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection */
SceneObjectIntersection Triangle::Intersects(const Ray3D &ray, double max_dist) const
{
    /* Initial test... are we even facing the triangle? */
    SceneObjectIntersection plane_intersect = Plane::Intersects(ray, max_dist);
    if (!plane_intersect.intersected) {
        return plane_intersect;
    }

    /* we are facing the plane containing this triangle. Figure out if
       we are interecting the triangle, or just the plane. */

    Vector3D v0v1 = this->verts[0].To(this->verts[1]),
        v0v2 = this->verts[0].To(this->verts[2]),
        p = ray.GetDir().Cross(v0v2);

    /* compute barycentric coords */
    Vector3D v0_to_origin = this->verts[0].To(ray.GetOrigin());
    double inverse_det = 1 / v0v1.Dot(p),
        u = v0_to_origin.Dot(p) * inverse_det;

    if (u > 1 || u < 0) {
        return SceneObjectIntersection(this, false, ray);
    }

    double v = ray.GetDir().Dot(v0_to_origin.Cross(v0v1)) * inverse_det;

    if (v < 0 || u + v > 1) {
        return SceneObjectIntersection(this, false, ray);
    }

    /* We successfully intersected the triangle! Since we already
       computed the intersection point and normal, return those. */
    return plane_intersect;
}

Box Triangle::GetBoundingBox() const
{
    return Box();
}
