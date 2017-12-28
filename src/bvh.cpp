#include "box.hpp"
#include "bvh.hpp"
#include "scene_object.hpp"
#include "zbuffer.hpp"

/* The BVH here is very simple; it's just a binary tree of BVHNodes,
 * each of which reference a scene object and its bounding box. A
 * child node's bounding box is completely enclosed by its parent's
 * bounding box.
 */
class BVHNode {
public:
    BVHNode();
    ~BVHNode();

    /* Check whether the given ray intersects this node. */
    Intersection Intersects(const Ray3D& ray,
                            double max_dist = INFINITY) const;

    std::vector<const SceneObject*>* objs;
    Box* bounding_box;
    BVHNode* left, * right;

    void AddObject(const SceneObject*);
    void Subdivide();

private:
    static const int MAX_OBJS = 3;
};

BVHNode::BVHNode() :
    objs(new std::vector<const SceneObject*>),
    bounding_box(new Box),
    left(nullptr),
    right(nullptr)
{
}

BVHNode::~BVHNode()
{
    delete objs;
    delete left;
    delete right;
    delete bounding_box;
}

void BVHNode::AddObject(const SceneObject* obj)
{
    objs->push_back(obj);
    bounding_box->Expand(obj->GetBoundingBox());
}

void BVHNode::Subdivide()
{
    /* Stack of child nodes to subdivide */
    std::vector<BVHNode*> node_stack;
    node_stack.push_back(this);

    while (!node_stack.empty()) {
        auto curr_node = node_stack.back();
        node_stack.pop_back();

        /* No need to subdivide if this node is small enough */
        if (!curr_node->objs || curr_node->objs->size() <= MAX_OBJS) {
            continue;
        }

        /* If we do have enough objects to divide, split along the mean
           position along the longest axis. */
        int longest_axis = curr_node->bounding_box->LongestAxis();

        double midpoint = 0;
        for (auto obj : *curr_node->objs) {
            midpoint += obj->GetPos().GetValue(longest_axis);
        }
        midpoint /= curr_node->objs->size();

       /* make left and right children */
        curr_node->left = new BVHNode;
        curr_node->right = new BVHNode;

        while (!curr_node->objs->empty()) {
            auto obj = curr_node->objs->back();
            curr_node->objs->pop_back();

            if (obj->GetPos().GetValue(longest_axis) > midpoint) {
                curr_node->right->AddObject(obj);
            } else {
                curr_node->left->AddObject(obj);
            }
        }

        delete curr_node->objs;
        curr_node->objs = nullptr;

        node_stack.push_back(curr_node->left);
        node_stack.push_back(curr_node->right);
    }
}

Intersection BVHNode::Intersects(const Ray3D& ray,
                                 double max_dist) const
{
    /* Check if we intersect the bounding box */
    return bounding_box->Intersects(ray, max_dist);
}

BVH::BVH(std::vector<const SceneObject*>& objs) :
    root(new BVHNode)
{
    /* Make sure the root box contains every object */
    for (auto obj : objs) {
        root->AddObject(obj);
    }

    /* Do a somewhat balanced subdivision of the root, recursively. */
    root->Subdivide();
}

BVH::~BVH()
{
    delete root;
}

SceneObjectIntersection BVH::Intersects(const Ray3D &ray, double max_dist) const
{
    /* This is essentially a stack holding nodes to do intersection
       checks on. */
    std::vector<const BVHNode*> to_check;
    to_check.push_back(root);

    const BVHNode* curr_node;
    Intersection curr_intersect(nullptr, false, ray);
    SceneObjectIntersection closest_obj_intersect(nullptr, false, ray);
    closest_obj_intersect.dist = INFINITY;
    while (!to_check.empty()) {
        curr_node = to_check.back();
        to_check.pop_back();

        /* An empty node got pushed, continue */
        if (!curr_node) {
            continue;
        }

        Intersection curr_intersect = curr_node->Intersects(ray, max_dist);

        /* Continue if no intersection. */
        if (!curr_intersect.intersected) {
            continue;
        }

        /* If this is a leaf node (i.e. this box references some
           objects), check intersection with that object and add to
           the z-buffer if successful */
        if (curr_node->objs) {
            for (auto obj : *curr_node->objs) {
                auto obj_intersect = obj->Intersects(ray, max_dist);
                if (obj_intersect.intersected &&
                    obj_intersect.dist < closest_obj_intersect.dist) {
                    closest_obj_intersect = obj_intersect;
                }
            }
        }

        to_check.push_back(curr_node->left);
        to_check.push_back(curr_node->right);
    }

    return closest_obj_intersect;
}
