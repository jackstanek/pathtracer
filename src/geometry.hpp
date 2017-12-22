#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "vector.hpp"

class Geometry {
protected:
    Geometry(const Vector3D& pos);
    Vector3D pos;
};

#endif
