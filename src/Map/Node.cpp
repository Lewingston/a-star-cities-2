
#include "Node.h"

#include <cmath>

using namespace asc2;

float Node::distance(const Node& node) const {

    const double lonDist = node.lon - lon;
    const double latDist = node.lat - lat;

    return static_cast<float>(std::sqrt(lonDist * lonDist + latDist * latDist));
}