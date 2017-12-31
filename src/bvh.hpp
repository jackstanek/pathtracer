#ifndef BVH_HPP_
#define BVH_HPP_

#include <queue>
#include <vector>

#include "box.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "scene_object.hpp"

/* The BVH here is very simple; it's just a binary tree of BVHNodes,
 * each of which reference a scene object and its bounding box. A
 * child node's bounding box is completely enclosed by its parent's
 * bounding box.
 */

class BVHNode {
public:
    BVHNode();

    /* Check whether the given ray intersects this node. */
    Intersection Intersects(const Ray3D& ray,
                            double max_dist = INFINITY) const;

    std::vector<const SceneObject*> objs;
    Box bounding_box;

    void AddObject(const SceneObject*);
};

class BVH {
public:
    BVH(std::vector<const SceneObject*>& objs);

    /* Get a record of closest object intersected by the given ray */
    SceneObjectIntersection Intersects(const Ray3D& ray, double max_dist) const;

private:
    static const int MAX_OBJS = 10;

    void Subdivide();
    std::vector<BVHNode> nodes;
};

#endif
