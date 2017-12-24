#ifndef INTERSECTION_HPP_
#define INTERSECTION_HPP_

#include "geometry.hpp"
#include "ray.hpp"
#include "scene_object.hpp"
#include "vector.hpp"

enum Incidence {
    INC_INWARD,
    INC_OUTWARD
};

struct Intersection {
    Intersection(const Geometry* obj,
                 bool intersected,
                 const Ray3D& along,
                 const Vector3D& point = Vector3D());

    const Geometry* obj;
    bool intersected;
    double dist;
};

struct SceneObjectIntersection : public Intersection {
    SceneObjectIntersection(const SceneObject* s_obj,
                            bool intersected,
                            const Ray3D& along,
                            int dir = 0,
                            const Vector3D& point = Vector3D(),
                            const Vector3D& norm = Vector3D(1, 0, 0));

    int inc;
    Vector3D point;
    Ray3D norm;

    inline bool operator> (const SceneObjectIntersection& i) const {
        return this->dist > i.dist;
    }

    /* Helper cast function */
    inline const SceneObject* GetObject() const {
        return static_cast<const SceneObject*>(obj);
    }
};

#endif
