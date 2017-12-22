#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include "color.hpp"
#include "scene_parser.hpp"
#include "vector.hpp"

class LightSource {
public:
    LightSource(Color intensity);
    virtual ~LightSource();

    virtual Color GetIntensity(const Vector3D& pt) const = 0;

    /* Get a vector pointing from point p to this light source */
    virtual Vector3D GetVecFromPoint(const Vector3D& pt) const = 0;

    /* Perceived distance from the light */
    virtual double Distance(const Vector3D& pt) const = 0;

    /* Construct light from */
    static LightSource* MakeFromComponent(SceneComponent* sc);

protected:
    Color intensity;
};

class PointLight : public LightSource {
public:
    PointLight(const Vector3D& pos, Color intensity);
    ~PointLight();

    virtual Color GetIntensity(const Vector3D& pt) const;
    virtual Vector3D GetVecFromPoint(const Vector3D& pt) const;
    virtual double Distance(const Vector3D& pt) const;

protected:
    Vector3D pos;
};

class SpotLight : public PointLight {
public:
    SpotLight(const Vector3D& pos, const Vector3D& dir, double in_angle, double ex_angle, Color intensity);
    ~SpotLight();

    virtual Color GetIntensity(const Vector3D& pt) const;

protected:
    Vector3D dir;
    double in_angle, ex_angle;
};

class DirectionalLight : public LightSource {
public:
    DirectionalLight(const Vector3D& dir, const Color& col);

    virtual Color GetIntensity(const Vector3D& pt) const;
    virtual Vector3D GetVecFromPoint(const Vector3D& pt) const;
    virtual double Distance(const Vector3D& pt) const;

protected:
    /* Passed to constructor pointing "toward" objects, but stored as
       pointing "toward" the light source for a slight bit of
       speed-up. */
    Vector3D dir;
};

#endif
