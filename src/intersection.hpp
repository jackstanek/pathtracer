#ifndef INTERSECTION_HPP_
#define INTERSECTION_HPP_

#include "ray.hpp"
#include "vector.hpp"

class SceneObject;

enum Incidence {
    INC_INWARD,
    INC_OUTWARD
};

struct Intersection {
    Intersection(const SceneObject* obj,
                 bool intersected,
                 const Ray3D& along,
                 int dir = 0,
                 const Vector3D& point = Vector3D(),
                 const Vector3D& norm = Vector3D(1, 0, 0));

    const SceneObject* obj;
    bool intersected;
    int inc;
    double dist;
    Vector3D point;
    Ray3D norm;

    inline bool operator> (const Intersection i) const {
        return this->dist > i.dist;
    }
};

#endif
