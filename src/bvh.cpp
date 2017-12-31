#include "box.hpp"
#include "bvh.hpp"
#include "scene_object.hpp"
#include "zbuffer.hpp"

BVHNode::BVHNode() :
    objs(),
    bounding_box()
{
}

void BVHNode::AddObject(const SceneObject* obj)
{
    objs.push_back(obj);
    bounding_box.Expand(obj->GetBoundingBox());
}

inline int left_child(int index)
{
    return 2 * index + 1;
}

inline int right_child(int index)
{
    return 2 * index + 2;
}

inline int parent(int index)
{
    return (index - 1) / 2;
}

void BVH::Subdivide()
{
    /* Stack of child nodes to subdivide */
    std::vector<int> node_index_stack;
    node_index_stack.push_back(0);

    /* Reserve enough space for the entire tree */

    while (!node_index_stack.empty()) {
        int curr_index = node_index_stack.back();
        node_index_stack.pop_back();

        BVHNode* curr_node = &nodes[curr_index];

        /* No need to subdivide if this node is small enough */
        if (curr_node->objs.size() <= MAX_OBJS ||
            right_child(curr_index) > nodes.size()) {
            continue;
        }

        /* If we do have enough objects to divide, split along the mean
           position along the longest axis. */
        int longest_axis = curr_node->bounding_box.LongestAxis();

        double midpoint = 0;
        for (auto obj : curr_node->objs) {
            midpoint += obj->GetPos().GetValue(longest_axis);
        }
        midpoint /= curr_node->objs.size();

        /* Add new child nodes */
        nodes[right_child(curr_index)] = BVHNode();
        nodes[left_child(curr_index)] = BVHNode();

        while (!curr_node->objs.empty()) {
            auto obj = curr_node->objs.back();
            curr_node->objs.pop_back();

            if (obj->GetPos().GetValue(longest_axis) < midpoint) {
                nodes[left_child(curr_index)].AddObject(obj);
            } else {
                nodes[right_child(curr_index)].AddObject(obj);
            }
        }

        node_index_stack.push_back(right_child(curr_index));
        node_index_stack.push_back(left_child(curr_index));
    }
}

Intersection BVHNode::Intersects(const Ray3D& ray,
                                 double max_dist) const
{
    /* Check if we intersect the bounding box */
    return bounding_box.Intersects(ray, max_dist);
}

BVH::BVH(std::vector<const SceneObject*>& objs) :
    nodes(2 * objs.size())
{
    /* Construct the root node of the BVH, stored at the first index
       of the array. */
    nodes[0] = BVHNode();

    /* Make sure the root box contains every object */
    for (auto obj : objs) {
        nodes[0].AddObject(obj);
    }

    /* Do a somewhat balanced subdivision of the root, recursively. */
    Subdivide();
}

SceneObjectIntersection BVH::Intersects(const Ray3D &ray, double max_dist) const
{
    /* This is essentially a stack holding nodes to do intersection
       checks on. */
    std::vector<int> to_check;
    to_check.push_back(0);

    size_t curr_node_index;
    Intersection curr_intersect(nullptr, false, ray);
    SceneObjectIntersection closest_obj_intersect(nullptr, false, ray);
    closest_obj_intersect.dist = INFINITY;

    while (!to_check.empty()) {
        curr_node_index = to_check.back();
        to_check.pop_back();

        if (curr_node_index >= nodes.size()) {
            continue;
        }

        auto curr_node = &nodes[curr_node_index];

        Intersection curr_intersect = curr_node->Intersects(ray, max_dist);

        /* Continue if no intersection. */
        if (!curr_intersect.intersected) {
            continue;
        }

        /* If this is a leaf node (i.e. this box references some
           objects), check intersection with that object and add to
           the z-buffer if successful */
        if (!curr_node->objs.empty()) {
            for (auto obj : curr_node->objs) {
                auto obj_intersect = obj->Intersects(ray, max_dist);
                if (obj_intersect.intersected &&
                    obj_intersect.dist < closest_obj_intersect.dist) {
                    closest_obj_intersect = obj_intersect;
                }
            }
        }

        to_check.push_back(left_child(curr_node_index));
        to_check.push_back(right_child(curr_node_index));
    }

    return closest_obj_intersect;
}
