#include "geometry.hpp"
#include "intersection.hpp"
#include "scene_object.hpp"

Intersection::Intersection(const Geometry* obj,
                           bool intersected,
                           const Ray3D& along,
                           const Vector3D& point) :
    obj(obj),
    intersected(intersected),
    dist((point - along.GetOrigin()).Norm())
{}


SceneObjectIntersection::SceneObjectIntersection(const SceneObject* obj,
                                                 bool intersected,
                                                 const Ray3D& along,
                                                 int inc,
                                                 const Vector3D& point,
                                                 const Vector3D& norm):
    Intersection(obj, intersected, along, point),
    inc(inc),
    point(point),
    norm(point, norm)
{
}
