
#include "Intersection.h"
#include "Road.h"

#include <algorithm>

using namespace asc2;

Intersection::Intersection(const Node& node) :
    node(node) {}

void Intersection::addRoad(const Road& rode) {

    roads.emplace_back(rode);
}

void Intersection::removeRoad(const Road& road) {

    roads.erase(std::remove_if(roads.begin(), roads.end(), [id = road.getId()](const Road& road) {
        return road.getId() == id;
    }), roads.end());
}