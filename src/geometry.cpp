#include "geometry.hpp"
#include "vector.hpp"

Geometry::Geometry(const Vector3D& pos) :
    pos(pos)
{}

Vector3D Geometry::GetPos() const
{
    return pos;
}
