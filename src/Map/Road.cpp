
#include "Road.h"

using namespace asc2;

void Road::addIntersection(const Intersection& intersection) {

    intersections.emplace_back(intersection);
}