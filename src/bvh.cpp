#include "box.hpp"
#include "bvh.hpp"
#include "scene_object.hpp"

BVHNode::BVHNode(const SceneObject* obj) :
    type(BNT_LEAF_NODE),
    obj(obj),
    bounding_box(obj->GetBoundingBox())
{
}

BVHNode::BVHNode(const BVHNode* left, const BVHNode* right) :
    type(BNT_BRANCH_NODE),
    obj(nullptr),
    bounding_box(new Box(*left->bounding_box, *right->bounding_box))
{
}

BVHNode::~BVHNode()
{
    if (type == BNT_BRANCH_NODE) {
        delete left;
        delete right;
        delete bounding_box;
    }
}

Intersection BVHNode::Intersects(const Ray3D& ray,
                                 double max_dist) const
{
    /* Bail if the ray doesn't intersect the bounding box */
    auto box_intersect = bounding_box->Intersects(ray, max_dist);
    if (box_intersect.intersected) {
        return Intersection(nullptr, false, ray);
    } else {
        return box_intersect;
    }
}

BVH::BVH(std::vector<const SceneObject*> objs) :
    root(ConstructTree(&objs, 0, objs.size()))
{
}

BVH::~BVH()
{
    delete root;
}

BVHNode* BVH::ConstructTree(std::vector<const SceneObject*>* objs,
                            int begin, int end)
{
    /* Sort by scene objects by their position projected onto the line
     * x = y = z */
    if (begin == end) {
        return new BVHNode((*objs)[begin]);
    } else {
        std::sort(objs->begin(), objs->end(),
                  [](const SceneObject* a, const SceneObject* b) {
                      return a->GetPos() < b->GetPos();
                  });

        int middle = (end - begin) / 2;
        return new BVHNode(ConstructTree(objs, begin, middle),
                           ConstructTree(objs, middle, end));
    }
}
