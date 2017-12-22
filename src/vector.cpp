#include <cassert>
#include <cmath>

#include "helper.hpp"
#include "vector.hpp"

Vector3D::Vector3D(double x, double y, double z) :
    x(x),
    y(y),
    z(z),
    norm(this->Norm())
{}

Vector3D::Vector3D(const SceneComponent* sc, int start_val)
{
    auto v = sc->values();

    this->x = v[start_val].d_val;
    this->y = v[start_val + 1].d_val;
    this->z = v[start_val + 2].d_val;
}

double Vector3D::GetX() const
{
    return this->x;
}

double Vector3D::GetY() const
{
    return this->y;
}

double Vector3D::GetZ() const
{
    return this->z;
}

Vector3D Vector3D::To(const Vector3D &v) const
{
    return v - *this;
}

double Vector3D::Norm() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Normalized() const
{
    if (is_approx(norm, 1)) {
        return *this;
    } else {
        return Vector3D(*this / this->Norm());
    }
}

double Vector3D::Dot(const Vector3D& v) const
{
    return this->x * v.x +
        this->y * v.y +
        this->z * v.z;
}

Vector3D Vector3D::Cross(const Vector3D& v) const
{
    return Vector3D(this->y * v.z - this->z * v.y,
                    this->z * v.x - this->x * v.z,
                    this->x * v.y - this->y * v.x);
}

Vector3D Vector3D::Projection(const Vector3D &v) const
{
    /* Scalar projection */
    double scal_proj = this->Dot(v) / this->Norm();
    return this->Normalized() * scal_proj;
}

Vector3D Vector3D::Halfway(const Vector3D &v) const
{
    return (*this + v).Normalized();
}

Vector3D Vector3D::ReflectAbout(const Vector3D& n) const
{
    return (n * 2 * (this->Dot(n)) - *this).Normalized();
}

Vector3D Vector3D::RefractThrough(const Vector3D& n, double ior) const
{
    double c = -n.Dot(this->Normalized());

    return n.Normalized()
        * (ior * c - std::sqrt(1 - ior * ior * (1 - c * c)))
        + this->Normalized() * ior;
}

Vector3D Vector3D::operator+ (const Vector3D& v) const
{
    return Vector3D(this->x + v.x,
                    this->y + v.y,
                    this->z + v.z);
}

void Vector3D::operator+= (const Vector3D& v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
}

Vector3D Vector3D::operator- (const Vector3D& v) const
{
    return Vector3D(this->x - v.x,
                    this->y - v.y,
                    this->z - v.z);
}

Vector3D Vector3D::operator- () const
{
    return Vector3D(-this->x,
                    -this->y,
                    -this->z);
}

Vector3D Vector3D::operator* (double c) const
{
    return Vector3D(this->x * c,
                    this->y * c,
                    this->z * c);
}

Vector3D Vector3D::operator/ (double c) const
{
    assert(c != 0.0);
    return Vector3D(this->x / c,
                    this->y / c,
                    this->z / c);
}
