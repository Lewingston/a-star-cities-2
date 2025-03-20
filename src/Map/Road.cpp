
#include "Road.h"
#include "Intersection.h"

using namespace asc2;

void Road::addIntersection(Intersection& intersection) {

    intersections.emplace_back(intersection);
}

std::vector<Road::NewRoadData> Road::splitRoadOnIntersections() const {

    std::vector<NewRoadData> newRoads;

    std::vector<std::reference_wrapper<Intersection>> newRoadIntersections;
    newRoadIntersections.emplace_back(intersections.front());

    //for (const Intersection& intersection : intersections) {
    for (auto iter = intersections.begin() + 1; iter != intersections.end() - 1; iter++) {

        newRoadIntersections.emplace_back(*iter);

        if (iter->get().getRoads().size() <= 1)
            continue;

        newRoads.emplace_back(type, newRoadIntersections);
        newRoadIntersections.clear();
        newRoadIntersections.emplace_back(*iter);
    }

    if (newRoadIntersections.size() > 0 && newRoads.size() > 0) {
        newRoadIntersections.emplace_back(intersections.back());
        newRoads.emplace_back(type, newRoadIntersections);
    }

    return newRoads;
}

void Road::removeFromIntersections() {

    for (Intersection& intersection : intersections) {
        intersection.removeRoad(*this);
    }
}