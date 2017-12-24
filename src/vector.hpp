#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <sstream>
#include <string>

#include "scene_parser.hpp"

enum Axis {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z,
    N_AXES
};

class Vector3D {
public:
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);
    Vector3D(const SceneComponent* sc, int start_val = 0);

    double GetX() const;
    double GetY() const;
    double GetZ() const;
    double GetValue(int axis) const;

    /* Vector from this to v */
    Vector3D To(const Vector3D& v) const;

    /* Norm of this vector */
    double Norm() const;

    /* Return a unit vector in the same direction */
    Vector3D Normalized() const;

    /* Dot product with vector v */
    double Dot(const Vector3D& v) const;

    /* Cross product with vector v */
    Vector3D Cross(const Vector3D& v) const;

    /* Projection of vector v onto this vector */
    Vector3D Projection(const Vector3D& v) const;

    /* Vector halfway between this one and v */
    Vector3D Halfway(const Vector3D& v) const;

    /* Reflect or refract about a normal vector */
    Vector3D ReflectAbout(const Vector3D& n) const;
    Vector3D RefractThrough(const Vector3D& n, double ior) const;

    /* Vector */

    Vector3D operator+ (const Vector3D& v) const;
    void operator+= (const Vector3D& v);
    Vector3D operator- (const Vector3D& v) const;
    Vector3D operator- () const;
    Vector3D operator* (double c) const;
    Vector3D operator/ (double c) const;

private:
    double vals[N_AXES];
    double norm;
};

Vector3D operator/ (double c, const Vector3D& v);

#endif
