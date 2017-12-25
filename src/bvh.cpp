#include "box.hpp"
#include "bvh.hpp"
#include "scene_object.hpp"
#include "zbuffer.hpp"

BVHNode::BVHNode(const SceneObject* obj) :
    type(BNT_LEAF_NODE),
    obj(obj),
    bounding_box(obj->GetBoundingBox()),
    left(nullptr),
    right(nullptr)
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
    /* Bail if the ray doesn't intersect the bounding box */
    auto box_intersect = bounding_box->Intersects(ray, max_dist);
    if (!box_intersect.intersected) {
        return Intersection(nullptr, false, ray);
    } else {
        return box_intersect;
    }
}

int BVHNode::GetNodeType() const
{
    return type;
}

const BVHNode* BVHNode::GetLeft() const
{
    return left;
}

const BVHNode* BVHNode::GetRight() const
{
    return right;
}

const SceneObject* BVHNode::GetObject() const
{
    return obj;
}

BVH::BVH(std::vector<const SceneObject*> objs)
{
    std::sort(objs.begin(), objs.end(),
              [](const SceneObject* a, const SceneObject* b) {
                  return a->GetPos() < b->GetPos();
              });
    root = ConstructTree(&objs, 0, objs.size());
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
    int len = end - begin;
    if (len == 1) {
        return new BVHNode(objs->at(begin));
    } else {
        int middle = len / 2 + begin;
        return new BVHNode(ConstructTree(objs, begin, middle),
                           ConstructTree(objs, middle, end));
    }
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
        if (curr_node->GetNodeType() == BNT_BRANCH_NODE) {
            to_check.push_back(curr_node->GetLeft());
            to_check.push_back(curr_node->GetRight());
        } else {
            /* If this is a leaf node (i.e. this box references an
               object), check intersection with that object and add to
               the z-buffer if successful */
            SceneObjectIntersection obj_intersect =
                curr_node->GetObject()->Intersects(ray, max_dist);
            zbuf.push(obj_intersect);
        }
    }

    if (!zbuf.empty()) {
        return zbuf.top();
    } else {
        return SceneObjectIntersection(nullptr, false, ray);
    }
}
