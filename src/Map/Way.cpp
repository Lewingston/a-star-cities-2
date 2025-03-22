
#include "Way.h"
#include "Intersection.h"

using namespace asc2;

Way::Way(uint64_t id, const std::vector<std::reference_wrapper<const Node>>& nodes, bool isComplete) :
    id(id),
    nodes(nodes),
    complete(isComplete) {}

float Way::getLength() const {

    float length = 0.0f;

    for (auto iter = nodes.begin(); iter != nodes.end() - 1; ++iter) {

        const Node& node = iter->get();
        const Node& nextNode = (iter++)->get();

        length += node.distance(nextNode);
    }

    return length;
}