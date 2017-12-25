#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "vector.hpp"

class Geometry {
public:
    Vector3D GetPos() const;
protected:
    Geometry(const Vector3D& pos);
    Vector3D pos;
};

#endif
