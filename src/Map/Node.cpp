
#include "Node.h"

#include <cmath>

using namespace asc2;

float Node::distance(const Node& node) const {

    const double lonDist = node.lon - lon;
    const double latDist = node.lat - lat;

    return static_cast<float>(std::sqrt(lonDist * lonDist + latDist * latDist));
}

std::pair<double, double> Node::pointBetween(const Node& node, float distance) const {

    const float totalDistance = this->distance(node);

    const float fraction = distance / totalDistance;

    const double x = lon + (node.lon - lon) * static_cast<double>(fraction);
    const double y = lat + (node.lat - lat) * static_cast<double>(fraction);

    return {x, y};
}