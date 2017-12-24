#include "intersection.hpp"
#include "sphere.hpp"

Sphere::Sphere(const Vector3D& pos, double radius, const Material& mat) :
    SceneObject(pos, mat),
    radius(radius)
{
    double box_extent = radius * M_SQRT2;
    Vector3D extent(box_extent, box_extent, box_extent);
    this->bounding_box = Box(-box_extent, box_extent);
}

Sphere::~Sphere()
{
}

SceneObjectIntersection Sphere::Intersects(const Ray3D& ray, double max_dist) const
{
    Vector3D to_ray_origin = ray.GetOrigin() - this->pos;

    /* object must be "in front of" the ray */
    if (ray.GetDir().Dot(-to_ray_origin) <= 0) {
        return SceneObjectIntersection(this, false, ray);
    }

    /* determinant */
    double base = -(ray.GetDir().Dot(to_ray_origin));
    double det = pow(ray.GetDir().Dot(to_ray_origin), 2)
        - pow(to_ray_origin.Norm(), 2)
        + pow(this->radius, 2);

    if (det < 0) {
        /* No intersection with sphere */
        return SceneObjectIntersection(this, false, ray);
    } else {
        /* Either one or two intersections; return closer one to camera */
        double s = sqrt(det);
        double t = s > base ? (base + s) : (base - s);

        /* Correct for "acne" by ignoring intersections at t=0 */
        if (is_zero(t)) {
            t = base + s;
        }

        if (t <= max_dist) {
            Vector3D int_pt = ray.Point(t);
            int inc = ray.GetDir().Dot(this->NormalAtPoint(int_pt)) < 0 ?
                INC_INWARD :
                INC_OUTWARD; // Incidence

            return SceneObjectIntersection(this,
                                           true,
                                           ray,
                                           inc,
                                           int_pt,
                                           this->NormalAtPoint(int_pt));
        } else {
            return SceneObjectIntersection(this, false, ray);
        }
    }
}
