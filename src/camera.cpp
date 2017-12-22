#include <cmath>

#include "camera.hpp"

Camera::Camera(double ha, uint32_t width, uint32_t height,
               const Vector3D& pos, const Vector3D& dir, const Vector3D& up) :
    pos(pos),
    dir(dir.Normalized()),
    up(up.Normalized()),
    right(dir.Cross(up).Normalized()),
    ha(ha * M_PI / 180),
    dist(height / (2.0 * std::tan(this->ha))),
    width(width),
    height(height)
{
}

Vector3D Camera::GetPos() const
{
    return this->pos;
}

Vector3D Camera::GetDir() const
{
    return this->dir;
}

Vector3D Camera::GetRight() const
{
    return this->right;
}

double Camera::GetHalfAngle() const
{
    return this->ha;
}

Ray3D Camera::GetRayThroughPoint(double x, double y) const
{
    Vector3D to_pix = (this->dir * this->dist - this->right * this->width * x +
                       this->dir * this->dist + this->up    * this->height * y) - this->pos;
    return Ray3D(this->pos, to_pix);
}

void Camera::Configure(SceneComponent *sc)
{
    ValueList v = sc->values();

    if (sc->key == CK_CAMERA) {
        this->pos = Vector3D(sc);
        this->dir = Vector3D(sc, 3).Normalized();
        this->up  = Vector3D(sc, 6).Normalized();
        this->right = dir.Cross(up).Normalized();
        this->ha  = v[9].d_val * M_PI / 180;
    } else if (sc->key == CK_FILM_RES) {
        this->width = v[0].i_val;
        this->height = v[1].i_val;
    }

    this->dist = this->height / (2.0 * std::tan(this->ha));
}
