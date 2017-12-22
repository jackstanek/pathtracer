#ifndef _SCENE_OBJECT_HPP
#define _SCENE_OBJECT_HPP

#include <vector>

#include "color.hpp"
#include "light.hpp"
#include "material.hpp"
#include "scene_object.hpp"
#include "vector.hpp"

struct Intersection;

class SceneObject {
public:
    SceneObject(const Vector3D& pos, const Material& mat);
    virtual ~SceneObject();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist = INFINITY) const = 0;
    virtual Vector3D NormalAtPoint(const Vector3D& v) const = 0;

    virtual Material GetMaterial() const;

    /* dynamically create a SceneObject from the SceneComponent */
    static SceneObject* MakeFromComponent(const SceneComponent* sc, const MaterialPool& mp);

protected:
    Vector3D pos;
    Material mat;
};

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

class Sphere : public SceneObject {
public:
    Sphere(const Vector3D& pos, double radius, const Material& mat);
    virtual ~Sphere();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;

private:
    double radius;

    virtual inline Vector3D NormalAtPoint(const Vector3D& v) const override
    {
        return this->pos.To(v).Normalized();
    };
};

class Plane : public SceneObject {
public:
    Plane(const Vector3D& origin, const Vector3D& normal, const Material& mat);
    virtual ~Plane();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;

    virtual inline Vector3D NormalAtPoint(const Vector3D& v) const override
    {
        return this->normal;
    }

protected:
    Vector3D normal;
};

class Triangle : public Plane {
public:
    Triangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3,
             const Material& mat);
    virtual ~Triangle();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;

protected:
    Vector3D verts[3];
};

class NormalTriangle : public Triangle {
public:
    NormalTriangle(const Vector3D& vert1, const Vector3D& vert2, const Vector3D& vert3,
                   const Vector3D& norm1, const Vector3D& norm2, const Vector3D& norm3,
                   const Material& mat);

    NormalTriangle(const Vector3D* verts_, const Vector3D* norms_, const Material& mat);

    virtual ~NormalTriangle();

    virtual Intersection Intersects(const Ray3D& ray, double max_dist) const override;
    virtual Vector3D NormalAtPoint(const Vector3D& v) const;

protected:
    Vector3D norms[3];
};

#endif
