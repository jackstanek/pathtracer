#include "box.hpp"
#include "bvh.hpp"
#include "scene_object.hpp"
#include "zbuffer.hpp"

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
    BVHNode(Box* bounding_box);
    BVHNode(const BVHNode* left, const BVHNode* right);
    ~BVHNode();

    /* Check whether the given ray intersects this node. */
    Intersection Intersects(const Ray3D& ray,
                            double max_dist = INFINITY) const;

    int type;
    const SceneObject* obj;
    const Box* bounding_box;
    const BVHNode* left, * right;
};

BVHNode::BVHNode(const SceneObject* obj) :
    type(BNT_LEAF_NODE),
    obj(obj),
    bounding_box(obj->GetBoundingBox()),
    left(nullptr),
    right(nullptr)
{
}

BVHNode::BVHNode(Box* bounding_box) :
    type(BNT_BRANCH_NODE),
    obj(nullptr)
{
}

BVHNode::BVHNode(const BVHNode* left, const BVHNode* right) :
    type(BNT_BRANCH_NODE),
    obj(nullptr),
    bounding_box(new Box(*left->bounding_box, *right->bounding_box)),
    left(left),
    right(right)
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
    /* Check if we intersect the bounding box */
    return bounding_box->Intersects(ray, max_dist);
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

}

SceneObjectIntersection BVH::Intersects(const Ray3D &ray, double max_dist) const
{
    /* This is essentially a stack holding nodes to do intersection
       checks on. */
    std::vector<const BVHNode*> to_check;
    to_check.push_back(root);

    const BVHNode* curr_node;
    Intersection curr_intersect(nullptr, false, ray);
    ZBuffer zbuf;
    while (!to_check.empty()) {
        curr_node = to_check.back();
        to_check.pop_back();

        Intersection curr_intersect = curr_node->Intersects(ray, max_dist);

        /* Continue if no intersection. */
        if (!curr_intersect.intersected) {
            continue;
        }

        /* If there's an intersection, add sub-boxes to the checking
           stack if this is a "branch" box */
        if (curr_node->type == BNT_BRANCH_NODE) {
            to_check.push_back(curr_node->left);
            to_check.push_back(curr_node->right);
        } else {
            /* If this is a leaf node (i.e. this box references an
               object), check intersection with that object and add to
               the z-buffer if successful */
            SceneObjectIntersection obj_intersect =
                curr_node->obj->Intersects(ray, max_dist);
            zbuf.push(obj_intersect);
        }
    }

    if (!zbuf.empty()) {
        return zbuf.top();
    } else {
        return SceneObjectIntersection(nullptr, false, ray);
    }
}
