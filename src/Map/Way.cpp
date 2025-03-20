
#include "Way.h"
#include "Intersection.h"

using namespace asc2;

Way::Way(uint64_t id, const std::vector<std::reference_wrapper<const Node>>& nodes, bool isComplete) :
    id(id),
    nodes(nodes),
    complete(isComplete) {}
