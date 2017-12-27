#include <cassert>
#include <cmath>

#include "helper.hpp"
#include "vector.hpp"

Vector3D::Vector3D(double x, double y, double z) :
    vals {x, y, z},
    norm(this->Norm())
{
}

Vector3D::Vector3D(const SceneComponent* sc, int start_val)
{
    auto v = sc->values();

    for (int axis = AXIS_X; axis < N_AXES; axis++) {
        vals[axis] = v[start_val + axis].d_val;
    }
}

double Vector3D::GetValue(int axis) const
{
    return this->vals[axis];
}

double Vector3D::GetX() const
{
    return this->GetValue(AXIS_X);
}

double Vector3D::GetY() const
{
    return this->GetValue(AXIS_Y);
}

double Vector3D::GetZ() const
{
    return this->GetValue(AXIS_Z);
}

Vector3D Vector3D::To(const Vector3D &v) const
{
    return v - *this;
}

double Vector3D::Norm() const
{
    return std::sqrt(vals[AXIS_X] * vals[AXIS_X] +
                     vals[AXIS_Y] * vals[AXIS_Y] +
                     vals[AXIS_Z] * vals[AXIS_Z]);
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
    /* I used getters here to improve readability, don't roast me for
       using getters within the class */
    return GetX() * v.GetX() +
        GetY() * v.GetY() +
        GetZ() * v.GetZ();
}

Vector3D Vector3D::Cross(const Vector3D& v) const
{
    /* Same deal here lol */
    return Vector3D(GetY() * v.GetZ() - GetZ() * v.GetY(),
                    GetZ() * v.GetX() - GetX() * v.GetZ(),
                    GetX() * v.GetY() - GetY() * v.GetX());
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

int Vector3D::LongestAxis() const
{
    return vals[AXIS_X] > vals[AXIS_Y] ?
        (vals[AXIS_Z] > vals[AXIS_X] ? AXIS_Z : AXIS_X) : AXIS_Y;
}

Vector3D Vector3D::MaxCombination(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(std::max(u.vals[AXIS_X], v.vals[AXIS_X]),
                    std::max(u.vals[AXIS_Y], v.vals[AXIS_Y]),
                    std::max(u.vals[AXIS_Z], v.vals[AXIS_Z]));
}

Vector3D Vector3D::MinCombination(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(std::min(u.vals[AXIS_X], v.vals[AXIS_X]),
                    std::min(u.vals[AXIS_Y], v.vals[AXIS_Y]),
                    std::min(u.vals[AXIS_Z], v.vals[AXIS_Z]));
}

Vector3D Vector3D::operator+ (const Vector3D& v) const
{
    /* Getters used within the class for readability, sorry */
    return Vector3D(GetX() + v.GetX(),
                    GetY() + v.GetY(),
                    GetZ() + v.GetZ());
}

void Vector3D::operator+= (const Vector3D& v)
{
    this->vals[AXIS_X] += v.GetX();
    this->vals[AXIS_Y] += v.GetY();
    this->vals[AXIS_Z] += v.GetZ();
}

Vector3D Vector3D::operator- (const Vector3D& v) const
{
    return Vector3D(GetX() - v.GetX(),
                    GetY() - v.GetY(),
                    GetZ() - v.GetZ());
}

Vector3D Vector3D::operator- () const
{
    return Vector3D(-GetX(),
                    -GetY(),
                    -GetZ());
}

Vector3D Vector3D::operator* (double c) const
{
    return Vector3D(GetX() * c,
                    GetY() * c,
                    GetZ() * c);
}

Vector3D Vector3D::operator/ (double c) const
{
    assert(c != 0.0);
    return Vector3D(GetX() / c,
                    GetY() / c,
                    GetZ() / c);
}

Vector3D operator/ (double c, const Vector3D& v)
{
    return Vector3D(c / v.GetX(),
                    c / v.GetY(),
                    c / v.GetZ());
}

bool Vector3D::operator< (const Vector3D& v) const
{
    for (int axis = AXIS_X; axis < N_AXES; axis++) {
        if (vals[axis] >= v.vals[axis]) {
            return false;
        }
    }

    return true;
}

bool Vector3D::operator<= (const Vector3D& v) const
{
    for (int axis = AXIS_X; axis < N_AXES; axis++) {
        if (vals[axis] > v.vals[axis]) {
            return false;
        }
    }

    return true;
}

bool Vector3D::operator== (const Vector3D& v) const
{
    for (int axis = AXIS_X; axis < N_AXES; axis++) {
        if (vals[axis] != v.vals[axis]) {
            return false;
        }
    }

    return true;
}
