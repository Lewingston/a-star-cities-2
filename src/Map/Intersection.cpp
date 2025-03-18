
#include "Intersection.h"

using namespace asc2;

Intersection::Intersection(const Node& node) :
    node(node) {}

void Intersection::addRoad(const Road& rode) {

    roads.emplace_back(rode);
}