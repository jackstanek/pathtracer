#include <cassert>
#include <cmath>

#include "helper.hpp"
#include "ray.hpp"
#include "vector.hpp"

Ray3D::Ray3D(const Vector3D& origin, const Vector3D& dir) :
    origin(origin),
    dir(dir.Normalized()),
    inv_dir(1 / this->dir)
{}

Vector3D Ray3D::Projection(const Vector3D &v) const
{
    Vector3D u = v - this->origin;
    return this->dir.Projection(u);
}

Vector3D Ray3D::GetOrigin() const
{
    return this->origin;
}

Vector3D Ray3D::GetDir() const
{
    return this->dir;
}


Vector3D Ray3D::GetInvDir() const
{
    return this->inv_dir;
}

Ray3D Ray3D::ReflectAbout(const Vector3D& pt, const Vector3D& n) const
{
    return Ray3D(pt, (-dir).ReflectAbout(n));
}

Ray3D Ray3D::RefractThrough(const Vector3D& pt, const Vector3D& n, double ior) const
{
    return Ray3D(pt, this->dir.RefractThrough(n, ior));
}

Vector3D Ray3D::Point(double t) const
{
    return this->origin + this->dir * t;
}
