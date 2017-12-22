#ifndef _SCENE_HPP
#define _SCENE_HPP

#include <string>
#include <vector>
#include <queue>
#include <stdint.h>

#include "camera.hpp"
#include "color.hpp"
#include "scene_object.hpp"

typedef std::priority_queue<Intersection, std::vector<Intersection>, std::greater<Intersection> > ZBuffer;

typedef std::vector<Vector3D> VertexPool;

class Scene {
public:
    Scene(uint32_t w = 640, uint32_t h = 480);
    ~Scene();

    void AddObject(const SceneObject* obj);
    void AddLight(const LightSource* light);

    uint32_t GetHeight() const;
    uint32_t GetWidth() const;

    void RenderPixels(int start, int stride, uint8_t* dst) const;

    inline Camera& GetCamera() {
        return this->cam;
    }

    void Configure(SceneComponent* sc);

private:
    /* Find what color lies at the end of ray */
    Color SceneColorAlongRay(const Ray3D& ray, uint8_t depth = 0) const;

    /* Compute the specific of some object at a given point */
    Color ObjectColorAtPoint(const Ray3D& view, const SceneObject* obj,
                             const Vector3D& pt, const Vector3D& normal,
                             uint8_t depth = 0) const;

    uint32_t width, height, num_pix;

    Camera cam;
    Color background, ambient;

    /* Distance from point to the image plane */
    double dist;

    std::vector<const SceneObject*> objects;
    std::vector<const LightSource*> lights;
};

#endif
