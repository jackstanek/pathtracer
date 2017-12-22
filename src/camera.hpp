#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <stdint.h>

#include "scene_parser.hpp"
#include "vector.hpp"

class Camera {
public:
    Camera(double ha,
           uint32_t width, uint32_t height,
           const Vector3D& pos = Vector3D(0.0, 0.0, 0.0),
           const Vector3D& dir = Vector3D(0.0, 0.0, 1.0),
           const Vector3D& up  = Vector3D(0.0, 1.0, 0.0));

    Vector3D GetPos() const;
    Vector3D GetDir() const;
    double GetHalfAngle() const;

    /* Get a unit vector pointing to the right of the camera */
    Vector3D GetRight() const;

    /* Get a ray intersecting the normalized point in x in [-1, 1] and
       y in [-1, 1] on the image plane */
    Ray3D GetRayThroughPoint(double x, double y) const;

    /* Scene description */
    void Configure(SceneComponent* sc);

private:
    Vector3D pos, dir, up, right;
    double ha;
    double dist;

    /* Resolution of the image */
    uint32_t width, height;
};

#endif
