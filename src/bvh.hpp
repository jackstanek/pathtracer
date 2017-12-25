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

enum BVHNodeType {
    BNT_BRANCH_NODE,
    BNT_LEAF_NODE
};

class BVHNode {
public:
    BVHNode(const SceneObject* obj);
    BVHNode(const BVHNode* left, const BVHNode* right);
    ~BVHNode();

    /* Check whether the given ray intersects this node. */
    Intersection Intersects(const Ray3D& ray,
                            double max_dist = INFINITY) const;

    int GetNodeType() const;
    const BVHNode* GetLeft() const;
    const BVHNode* GetRight() const;
    const SceneObject* GetObject() const;

private:
    int type;
    const SceneObject* obj;
    const Box* bounding_box;
    const BVHNode* left, * right;
};

class BVH {
public:
    BVH(std::vector<const SceneObject*> objs);
    ~BVH();

    /* Get a record of closest object intersected by the given ray */
    SceneObjectIntersection Intersects(const Ray3D& ray, double max_dist) const;

private:
    BVHNode* root;
    BVHNode* ConstructTree(std::vector<const SceneObject*>* objs,
                           int begin, int end);
};

#endif
