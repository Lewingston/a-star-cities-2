
#include "Intersection.h"
#include "Road.h"

#include <algorithm>
#include <iostream>

using namespace asc2;

Intersection::Intersection(const Node& node) :
    node(node) {}

void Intersection::addRoad(Road& rode) {

    roads.emplace_back(rode);
}

void Intersection::removeRoad(const Road& road) {

    roads.erase(std::remove_if(roads.begin(), roads.end(), [id = road.getId()](const Road& road) {
        return road.getId() == id;
    }), roads.end());
}

std::vector<Intersection::Connection> Intersection::getConnections(bool calculateDistance) const {

    std::vector<Connection> connections;
    connections.reserve(roads.size());

    for (const Road& road : roads) {

        const float roadLength = calculateDistance ? road.getLength() : 0.0f;

        if (road.getIntersections().front().get() == *this) {
            connections.emplace_back(
                road.getIntersections().back(),
                road,
                roadLength
            );
        } else if (road.getIntersections().back().get() == *this) {
            connections.emplace_back(
                road.getIntersections().front(),
                road,
                roadLength
            );
        } else {
            std::cerr << "Unable to get connection - Road id: " << road.getId() << '\n';
        }
    }

    return connections;
}