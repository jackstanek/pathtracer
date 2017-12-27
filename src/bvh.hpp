#ifndef BVH_HPP_
#define BVH_HPP_

#include <queue>
#include <vector>

#include "box.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "scene_object.hpp"

class BVHNode;

class BVH {
public:
    BVH(std::vector<const SceneObject*>& objs);
    ~BVH();

    /* Get a record of closest object intersected by the given ray */
    SceneObjectIntersection Intersects(const Ray3D& ray, double max_dist) const;

private:
    BVHNode* root;
};

#endif
