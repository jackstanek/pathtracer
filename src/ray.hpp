#ifndef RAY_HPP_
#define RAY_HPP_

#include "vector.hpp"

class Ray3D {
public:
    Ray3D(const Vector3D& origin, const Vector3D& dir);

    Vector3D GetOrigin() const;
    Vector3D GetDir() const;
    Vector3D GetInvDir() const;

    /* Get a point on the ray at parameter t, or at point on axis at pt */
    Vector3D Point(double t) const;
    Vector3D Point(double pt, int axis) const;

    /* Project a point at vector v onto this ray and return the
       projected point */
    Vector3D Projection(const Vector3D& v) const;

    /* Reflect or refract an *incoming* ray about some normal */
    Ray3D ReflectAbout(const Vector3D& pt, const Vector3D& n) const;
    Ray3D RefractThrough(const Vector3D& pt, const Vector3D& n, double ior) const;

private:
    Vector3D origin;
    Vector3D dir;

    /* Reciprocal of the direction vector */
    Vector3D inv_dir;
};

#endif
