#include <algorithm>
#include <cmath>

#include "color.hpp"
#include "scene_object.hpp"
#include "vector.hpp"

Intersection::Intersection(const SceneObject* obj,
                           bool intersected,
                           const Ray3D& along,
                           int inc,
                           const Vector3D& point,
                           const Vector3D& norm):
    obj(obj),
    intersected(intersected),
    inc(inc),
    dist((point - along.GetOrigin()).Norm()),
    point(point),
    norm(point, norm)
{
}

SceneObject::SceneObject(const Vector3D& pos, const Material& mat) :
    pos(pos),
    mat(mat)
{
}

SceneObject::~SceneObject()
{
}

Sphere::Sphere(const Vector3D& pos, double radius, const Material& mat) :
    SceneObject(pos, mat),
    radius(radius)
{
}

Material SceneObject::GetMaterial() const
{
    return this->mat;
}

SceneObject* SceneObject::MakeFromComponent(const SceneComponent* sc, const MaterialPool& mp)
{
    auto v = sc->values();

    switch (sc->key) {
    case CK_SPHERE:
        Vector3D pos(sc);
        return new Sphere(pos, v[3].d_val, mp.back());
    }

    return nullptr;
}

Sphere::~Sphere()
{
}

Intersection Sphere::Intersects(const Ray3D& ray, double max_dist) const
{
    Vector3D to_ray_origin = ray.GetOrigin() - this->pos;

    /* object must be "in front of" the ray */
    if (ray.GetDir().Dot(-to_ray_origin) <= 0) {
        return Intersection(this, false, ray);
    }

    /* determinant */
    double base = -(ray.GetDir().Dot(to_ray_origin));
    double det = pow(ray.GetDir().Dot(to_ray_origin), 2)
        - pow(to_ray_origin.Norm(), 2)
        + pow(this->radius, 2);

    if (det < 0) {
        /* No intersection with sphere */
        return Intersection(this, false, ray);
    } else {
        /* Either one or two intersections; return closer one to camera */
        double s = sqrt(det);
        double t = s > base ? (base + s) : (base - s);

        /* Correct for "acne" by ignoring intersections at t=0 */
        if (is_zero(t)) {
            t = base + s;
        }

        if (t <= max_dist) {
            Vector3D int_pt = ray.Point(t);
            int inc = ray.GetDir().Dot(this->NormalAtPoint(int_pt)) < 0 ?
                INC_INWARD :
                INC_OUTWARD; // Incidence

            return Intersection(this,
                                true,
                                ray,
                                inc,
                                int_pt,
                                this->NormalAtPoint(int_pt));
        } else {
            return Intersection(this, false, ray);
        }
    }
}

Plane::Plane(const Vector3D& pos, const Vector3D& normal, const Material& mat) :
    SceneObject(pos, mat),
    normal(normal.Normalized())
{
}

Plane::~Plane()
{
}

Intersection Plane::Intersects(const Ray3D &ray, double max_dist) const
{
    double denom = ray.GetDir().Dot(this->normal);

    /* Infinite / zero intersections if ray perpendicular to normal */
    if (is_zero(denom)) {
        return Intersection(this, false, ray);
    } else {
        double t = (this->pos - ray.GetOrigin()).Dot(this->normal) / denom;

        if (t >= EPSILON && t <= max_dist) {
            return Intersection(this,
                                true,
                                ray,
                                INC_INWARD,
                                ray.Point(t),
                                this->normal);
        } else {
            return Intersection(this, false, ray);
        }
    }
}

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
Intersection Triangle::Intersects(const Ray3D &ray, double max_dist) const
{
    /* Initial test... are we even facing the triangle? */
    Intersection plane_intersect = Plane::Intersects(ray, max_dist);
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
        return Intersection(this, false, ray);
    }

    double v = ray.GetDir().Dot(v0_to_origin.Cross(v0v1)) * inverse_det;

    if (v < 0 || u + v > 1) {
        return Intersection(this, false, ray);
    }

    /* We successfully intersected the triangle! Since we already
       computed the intersection point and normal, return those. */
    return plane_intersect;
}

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

Intersection NormalTriangle::Intersects(const Ray3D &ray, double max_dist) const
{
    Intersection intersected = Triangle::Intersects(ray, max_dist);
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
