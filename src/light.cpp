#include "helper.hpp"
#include "light.hpp"

LightSource::LightSource(Color intensity) :
    intensity(intensity)
{
}

LightSource::~LightSource()
{
}

LightSource* LightSource::MakeFromComponent(SceneComponent* sc)
{
    ValueList v = sc->values();
    Color col(sc);
    Vector3D pos(sc, 3);

    switch (sc->key) {
    case CK_POINT_LIGHT:
        return new PointLight(pos, col);
    case CK_SPOT_LIGHT:
        return new SpotLight(pos, Vector3D(sc, 6), v[9].d_val, v[10].d_val, col);
    case CK_DIRECTIONAL_LIGHT:
        return new DirectionalLight(pos, col);
    default:
        return new PointLight(pos, v[0].d_val);
    }
}

PointLight::PointLight(const Vector3D& pos, Color intensity) :
    LightSource(intensity),
    pos(pos)
{
}

PointLight::~PointLight()
{
}

Color PointLight::GetIntensity(const Vector3D& pt) const
{
    double dist = this->Distance(pt);
    return this->intensity * (1 / (dist * dist));
}

Vector3D PointLight::GetVecFromPoint(const Vector3D& p) const
{
    return (this->pos - p);
}

double PointLight::Distance(const Vector3D& pt) const
{
    return pt.To(this->pos).Norm();
}

SpotLight::SpotLight(const Vector3D& pos, const Vector3D& dir, double in_angle, double ex_angle, Color intensity) :
    PointLight(pos, intensity),
    dir(dir.Normalized()),
    in_angle(in_angle),
    ex_angle(ex_angle)
{
    assert(ex_angle < 360 && ex_angle > in_angle && in_angle > 0);
}

SpotLight::~SpotLight()
{
}

Color SpotLight::GetIntensity(const Vector3D& pt) const
{
    double angle = this->pos.To(pt).Normalized().Dot(this->dir) * 180 / M_PI;
    if (angle < in_angle) {
        return PointLight::GetIntensity(pt);
    } else if (angle > ex_angle) {
        return Color();
    } else {
        /* falloff */
        return PointLight::GetIntensity(pt) * (angle - in_angle) * (1 / (ex_angle - in_angle));
    }
}

DirectionalLight::DirectionalLight(const Vector3D& dir, const Color& intensity) :
    LightSource(intensity),
    dir(-dir)
{
}

Color DirectionalLight::GetIntensity(const Vector3D& pt) const
{
    return this->intensity;
}

Vector3D DirectionalLight::GetVecFromPoint(const Vector3D& pt) const
{
    return this->dir;
}

double DirectionalLight::Distance(const Vector3D& pt) const
{
    return INFINITY;
}
