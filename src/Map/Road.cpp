
#include "Road.h"
#include "Intersection.h"

#include <algorithm>

using namespace asc2;

float Road::getLength() const {

    return way.getLength();
}

void Road::addIntersection(Intersection& intersection) {

    intersections.emplace_back(intersection);
}

void Road::addIntersectionToFront(Intersection& intersection) {

    intersections.emplace(intersections.begin(), intersection);
}

void Road::removeIntersection(const Intersection& intersection) {

    intersections.erase(std::remove_if(intersections.begin(), intersections.end(), [id = intersection.getNode().id](const Intersection& intersection) {
        return intersection.getNode().id == id;
    }), intersections.end());
}

std::vector<Road::NewRoadData> Road::splitRoadOnIntersections() const {

    std::vector<NewRoadData> newRoads;

    if (intersections.size() == 0)
        return newRoads;

    auto intersectionIter = intersections.begin();

    NewRoadData newRoad{type, {}, {}};

    for (auto nodeIter = way.getNodes().begin(); nodeIter != way.getNodes().end(); ++nodeIter) {

        newRoad.nodes.emplace_back(nodeIter->get());

        if (intersectionIter != intersections.end() && nodeIter->get().id == intersectionIter->get().getId()) {
            newRoad.intersections.emplace_back(intersectionIter->get());

            const bool split = nodeIter != way.getNodes().begin() &&
                               nodeIter != way.getNodes().end() -1 &&
                               intersectionIter->get().getRoads().size() >= 2;

            if (!split) {
                ++intersectionIter;
                continue;
            }

        } else {
            
            continue;
        }

        newRoads.push_back(newRoad);
        newRoad = NewRoadData(type, {nodeIter->get()}, {intersectionIter->get()});

        ++intersectionIter;
    }

    if (newRoad.nodes.size() > 1)
        newRoads.push_back(newRoad);

    if (newRoads.size() > 1)
        return newRoads;
    else
        return {};
}

Road::NewRoadData Road::fuse(const Road& road) const {

    NewRoadData newRoad{type, {}, {}};

    if (way.getNodes().front().get().id == road.way.getNodes().front().get().id) {

        newRoad.nodes.insert(newRoad.nodes.end(), way.getNodes().rbegin(), way.getNodes().rend());
        if (road.way.getNodes().size() > 1)
            newRoad.nodes.insert(newRoad.nodes.end(), road.way.getNodes().begin() + 1, road.way.getNodes().end());

        newRoad.intersections.insert(newRoad.intersections.end(), intersections.rbegin(), intersections.rend());
        if (road.intersections.size() > 1)
            newRoad.intersections.insert(newRoad.intersections.end(), road.intersections.begin() + 1, road.intersections.end());

    } else if (way.getNodes().front().get().id == road.way.getNodes().back().get().id) {

        newRoad.nodes.insert(newRoad.nodes.end(), way.getNodes().rbegin(), way.getNodes().rend());
        if (road.way.getNodes().size() > 1)
            newRoad.nodes.insert(newRoad.nodes.end(), road.way.getNodes().rbegin() + 1, road.way.getNodes().rend());

        newRoad.intersections.insert(newRoad.intersections.end(), intersections.rbegin(), intersections.rend());
        if (road.intersections.size() > 1)
            newRoad.intersections.insert(newRoad.intersections.end(), road.intersections.rbegin() + 1, road.intersections.rend());

    } else if (way.getNodes().back().get().id == road.way.getNodes().front().get().id) {

        newRoad.nodes.insert(newRoad.nodes.end(), way.getNodes().begin(), way.getNodes().end());
        if (road.way.getNodes().size() > 1)
            newRoad.nodes.insert(newRoad.nodes.end(), road.way.getNodes().begin() + 1, road.way.getNodes().end());

        newRoad.intersections.insert(newRoad.intersections.end(), intersections.begin(), intersections.end());
        if (road.intersections.size() > 1)
            newRoad.intersections.insert(newRoad.intersections.end(), road.intersections.begin() + 1, road.intersections.end());

    } else if (way.getNodes().back().get().id == road.way.getNodes().back().get().id) {

        newRoad.nodes.insert(newRoad.nodes.end(), way.getNodes().begin(), way.getNodes().end());
        if (road.way.getNodes().size() > 1)
            newRoad.nodes.insert(newRoad.nodes.end(), road.way.getNodes().rbegin() + 1, road.way.getNodes().rend());

        newRoad.intersections.insert(newRoad.intersections.end(), intersections.begin(), intersections.end());
        if (road.intersections.size() > 1)
            newRoad.intersections.insert(newRoad.intersections.end(), road.intersections.rbegin() + 1, road.intersections.rend());
    }

    return newRoad;
}

void Road::removeFromIntersections() {

    for (Intersection& intersection : intersections) {
        intersection.removeRoad(*this);
    }
}