#include <algorithm>
#include <iostream>
#include <cmath>
#include <mutex>
#include <thread>
#include <vector>
#include <stdint.h>

#include "camera.hpp"
#include "intersection.hpp"
#include "scene.hpp"
#include "scene_object.hpp"

#define MAX_DEPTH (10)

Scene::Scene(uint32_t w,
             uint32_t h) :
    width(w),
    height(h),
    num_pix(w * h),
    cam(45, width, height)
{
}

Scene::~Scene()
{
    for (auto obj : this->objects) {
        delete obj;
    }

    for (auto light : this->lights) {
        delete light;
    }
}

void Scene::AddObject(SceneObject* obj)
{
    this->objects.push_back(obj);
}

void Scene::AddLight(LightSource* light)
{
    this->lights.push_back(light);
}

Color Scene::ObjectColorAtPoint(const Ray3D& view,
                                const SceneObject* obj,
                                const Vector3D& pt,
                                const Vector3D& normal,
                                uint8_t depth) const
{
    Color acc;
    Material mat = obj->GetMaterial();

    for (auto light : this->lights) {
        /* Lambertian shading... TODO: Factor lighting models into
           their own class hierarchy */
        Vector3D obj_to_light = light->GetVecFromPoint(pt);
        Color diffuse = mat.diffuse
             * std::max(0.0, obj_to_light.Normalized().Dot(normal))
            * light->GetIntensity(pt);

        /* Phong specular shading */
        Vector3D ref_light = obj_to_light.Normalized().ReflectAbout(normal),
            obj_to_cam = pt.To(this->cam.GetPos()).Normalized();

        Color specular = mat.specular
            * std::pow(std::max(0.0, obj_to_cam.Dot(ref_light)), mat.phong_exp)
            * light->GetIntensity(pt);

        /* shadows */
        bool blocked = false;
        for (auto obj : this->objects) {
            if (obj->Intersects(Ray3D(pt, obj_to_light), light->Distance(pt)).intersected) {
                blocked = true;
            }
        }

        if (!blocked) {
            acc += diffuse + specular;
        }
    }

    /* Reflective component */
    acc += this->SceneColorAlongRay(view.ReflectAbout(pt, normal), depth + 1) * mat.specular;

    /* Refractive component */
    acc += this->SceneColorAlongRay(view.RefractThrough(pt, normal, 1 / mat.ior), depth + 1) * mat.transmissivity;

    /* Ambient component */
    acc += mat.ambient * this->ambient;

    return acc;
}

Color Scene::SceneColorAlongRay(const Ray3D& ray, uint8_t depth) const
{
    if (depth > MAX_DEPTH) {
        return Color(0, 0, 0);
    }

    ZBuffer zb;
    //Intersection closest;

    for (auto obj : this->objects) {
        Intersection info = obj->Intersects(ray);

        if (info.intersected) {
            zb.push(info);
        }
    }

    /* >= 1 object intersected */
    if (zb.size() > 0) {
        Intersection closest = zb.top();

        if (closest.inc == INC_INWARD) {
            return this->ObjectColorAtPoint(ray,
                                            closest.obj,
                                            closest.point,
                                            closest.norm.GetDir(),
                                            depth);
        } else if (closest.inc == INC_OUTWARD) {
            return this->SceneColorAlongRay(ray.RefractThrough(closest.point,
                                                               -closest.norm.GetDir(),
                                                               closest.obj->GetMaterial().ior),
                                            depth + 1);
        }
    }

    /* No object intersected; ray exits scene. default
       to black (ie no light reflected) */
    return this->background;
}

inline std::vector<double> extract_luminances(std::vector<Color> colors)
{
    std::vector<double> lums(colors.size());
    for (uint32_t i = 0; i < colors.size(); i++) {
        lums[i] = colors[i].Luminance();
    }
    return lums;
}

static std::mutex rend_ct_lock;

void Scene::RenderPixels(int start, int stride, uint8_t* dst) const
{
    static uint32_t rendered; /* pixels rendered */

    for (uint32_t i = start; i < num_pix; i += stride) {
        int y = i / this->width, x = i % this->width;

        std::vector<Color> samples;
        std::vector<double> lums;

        /* floating-point offsets from the center of the image to the
           right and top edges */
        double xoff = this->width / 2.0, yoff = this->height / 2.0;

        double lum_range = INFINITY;

        do {
            Ray3D ray = cam.GetRayThroughPoint((x - xoff + rand_d())  / xoff,
                                               -(y - yoff + rand_d()) / yoff);
            samples.push_back(this->SceneColorAlongRay(ray));
            lum_range = range(extract_luminances(samples));
        } while (samples.size() < 8 || lum_range / samples.size() > 0.01);

        /* Output the linear average of the samples to the
           pixel. */
        Color::Average(samples).Output8BitPixel(dst + i * 4);

        /* Print the status marker */
        rend_ct_lock.lock();
        rendered++;
        if (rendered % 100 == 0) {
            std::printf("\r[");
            double ratio = (double) rendered / this->num_pix;
            for (int i = 0; i < 20 * ratio; i++) {
                std::printf("#");
            }
            for (int i = 0; i < 20 * (1 - ratio); i++) {
                std::printf("-");
            }
            std::printf("] (%d/%d)", rendered, this->num_pix);
        }
        rend_ct_lock.unlock();
    }
}

void Scene::Configure(SceneComponent *sc)
{
    ValueList v = sc->values();

    if (sc->key == CK_FILM_RES) {
        this->width = v[0].i_val;
        this->height = v[1].i_val;
        this->num_pix = this->width * this->height;
        this->cam.Configure(sc);
    } else if (sc->key == CK_BACKGROUND) {
        this->background = Color(v[0].d_val, v[1].d_val, v[2].d_val);
    } else if (sc->key == CK_AMBIENT_LIGHT) {
        this->ambient = Color(v[0].d_val, v[1].d_val, v[2].d_val);
    }
}

uint32_t Scene::GetWidth() const
{
    return this->width;

}

uint32_t Scene::GetHeight() const
{
    return this->height;
}
