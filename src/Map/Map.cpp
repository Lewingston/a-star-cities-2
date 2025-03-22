
#include "Map.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace asc2;

void Map::addNode(const Node& node) {

    idHandler.updateUsedIds(node.id);
    const auto [iter, success] = nodes.emplace(node.id, node);
    if (!success) {
        std::cerr << "Failed to add node with id: " + std::to_string(node.id) +  " to map!\n";
    } else {
        dimensions.adjust(node.lon, node.lat);
    }
}

void Map::addWay(const Way& way) {

    idHandler.updateUsedIds(way.getId());
    const auto [iter, success] = ways.emplace(way.getId(), way);
    if (!success) {
        std::cerr << "Failed to add way with id: " + std::to_string(way.getId()) + " to map!\n";
    }
}

void Map::addRoad(RoadType type, uint64_t wayId) {

    if (auto find = ways.find(wayId); find != ways.end()) {
        addRoad(type, find->second);
    } else {
        std::cerr << "Failed to add road with id: " << wayId << " - Way not found!\n";
    }
}

void Map::addRoad(RoadType type, const Way& way) {

    auto [iter, success] = roads.emplace(way.getId(), Road(type, way));
    if (!success) {
        std::cerr << "Failed to add road with id: " << way.getId() << " - Duplicate road in map\n";
        return;
    }

    Road& road = iter->second;

    // add new intersections for all nodes in road
    for (const Node& node : road.getWay().getNodes()) {

        auto [iter, success] = intersections.insert({node.id, Intersection(node)});
        iter->second.addRoad(road);
        road.addIntersection(iter->second);
    }
}

void Map::addBuilding(BuildingType type, uint64_t wayId) {

    if (auto find = ways.find(wayId); find != ways.end()) {
        buildings.push_back(Building(type, find->second));
        idHandler.updateUsedIds(wayId);
    } else {
        std::cerr << "Failed to add building with id: " << wayId << " - Way not found!\n";
    }
}

void Map::addBuilding(BuildingType type,
                      uint64_t relationId,
                      const std::vector<uint64_t>& outerWayIds,
                      const std::vector<uint64_t>& innerWayIds) {

    std::vector<std::reference_wrapper<const Way>> outerWays;
    outerWays.reserve(outerWayIds.size());
    std::vector<std::reference_wrapper<const Way>> innerWays;
    innerWays.reserve(innerWayIds.size());

    for (uint64_t id : outerWayIds) {
        if (auto find = this->ways.find(id); find != ways.end()) {
            outerWays.emplace_back(find->second);
        } else {
            std::cerr << "Failed to add building with id: " << relationId << " - Inner way not found: " << id << '\n';
            return;
        }
    }

    for (uint64_t id : innerWayIds) {
        if (auto find = this->ways.find(id); find != ways.end()) {
            innerWays.emplace_back(find->second);
        } else {
            std::cerr << "Failed to add building with id: " << relationId << " - Outer way not found: " << id << '\n';
            return;
        }
    }

    buildings.push_back(Building{
        type,
        relationId,
        outerWays,
        innerWays
    });
}

void Map::optimizeIntersecions() {

    removeUnnecessaryIntersections();
    splitRoadsOnIntersections();
    fuseRoads();
}

void Map::splitRoadsOnIntersections() {

    std::vector<Road::NewRoadData> newRoads;
    std::vector<std::reference_wrapper<Road>> removeRoads;

    // iterate over all roads
    for (auto& [id, road] : roads) {

        const std::vector<Road::NewRoadData> roads = road.splitRoadOnIntersections();
        newRoads.insert(newRoads.begin(), roads.begin(), roads.end());

        if (roads.size() > 0) {
            removeRoads.emplace_back(road);
        }
    }

    // remove all old roads there ways
    for (Road& road : removeRoads) {
        removeRoadAndWay(road);
    }

    // add new roads and ways
    for (const auto& data : newRoads) {
        addRoadAndWay(data);
    }

    std::cout << removeRoads.size() << " roads removed.\n";
    std::cout << newRoads.size() << " new roads added.\n";
}

void Map::removeIntersection(Intersection& intersection) {

    for (Road& road : intersection.getRoads()) {
        road.removeIntersection(intersection);
    }

    intersections.erase(intersection.getId());
}

void Map::removeRoadAndWay(Road& road) {

    const uint64_t roadId = road.getId();
    const uint64_t wayId = road.getWay().getId();

    road.removeFromIntersections();
    roads.erase(roadId);
    ways.erase(wayId);
}

void Map::addRoadAndWay(const Road::NewRoadData& data) {

    const uint64_t newId = idHandler.getNewId();

    // add new way
    const auto [wayIter, waySuccess] = ways.emplace(newId, Way(newId, data.nodes, true));
    if (!waySuccess) {
        std::cerr << "Failed to insert way: " << newId << '\n';
        return;
    }

    // add new road
    const auto [roadIter, roadSuccess] = roads.emplace(newId, Road(data.type, wayIter->second));
    if (!roadSuccess) {
        std::cerr << "Failed to insert road: " << newId << '\n';
        return;
    }

    // add road to intersections and intersections to road
    for (auto& intersection : data.intersections) {
        intersection.get().addRoad(roadIter->second);
        roadIter->second.addIntersection(intersection);
    }
}

void Map::removeUnnecessaryIntersections() {

    std::vector<std::reference_wrapper<Intersection>> removeIntersections;
    for (auto& [id, intersection] : intersections) {

        if (intersection.getRoads().size() >= 2)
            continue;

        removeIntersections.emplace_back(intersection);
    }

    for (Intersection& intersection : removeIntersections) {
        removeIntersection(intersection);
    }

    std::cout << "Unnecessary intersections removed: " << removeIntersections.size() << '\n';
}

void Map::fuseRoads() {

    std::vector<std::reference_wrapper<Intersection>> removeIntersections;

    for (auto& [id, intersection] : intersections) {

        if (intersection.getRoads().size() != 2) {
            continue;
        }

        Road& r1 = intersection.getRoads().front().get();
        Road& r2 = intersection.getRoads().back().get();

        if (r1.getId() == r2.getId()) {
            continue;
        }

        if (r1.getType() != r2.getType()) {
            continue;
        }

        const Road::NewRoadData newRoad = r1.fuse(r2);
        if (newRoad.nodes.size() == 0) {
            std::cout << "Unable to fuse roads: " << r1.getId() << " - " << r2.getId() << '\n';
            continue;
        }

        removeRoadAndWay(r1);
        removeRoadAndWay(r2);
        addRoadAndWay(newRoad);

        removeIntersections.emplace_back(intersection);
    }

    for (auto inter : removeIntersections) {
        removeIntersection(inter);
    }

    std::cout << "Fused intersections: " << removeIntersections.size() << '\n';
}

void Map::addIntersectionsToEndPoints() {
    
    for (auto& [id, road] : roads) {

    }
}

std::vector<std::reference_wrapper<const Node>> Map::getNodes(
    const std::vector<uint64_t>& ids) const {

    std::vector<std::reference_wrapper<const Node>> nodes;
    nodes.reserve(ids.size());

    for (uint64_t id : ids) {

        if (auto find = this->nodes.find(id); find != this->nodes.end()) {
            nodes.emplace_back(find->second);
        }
    }

    return nodes;
}

std::vector<std::reference_wrapper<const Way>> Map::getWays(
    const std::vector<uint64_t>& ids) const {

    std::vector<std::reference_wrapper<const Way>> ways;
    ways.reserve(ids.size());

    for (uint64_t id : ids) {

        if (auto find = this->ways.find(id); find != this->ways.end()) {
            ways.emplace_back(find->second);
        }
    }

    return ways;
}

std::pair<double, double> Map::getCenter() const {

    return dimensions.getCenter();
}

/************************************************************/
/*                   struct Map::Dimensions                 */
/************************************************************/

void Map::Dimensions::adjust(double lon, double lat) {

    if (lon < minLon)
        minLon = lon;
    else if (lon > maxLon)
        maxLon = lon;

    if (lat < minLat)
        minLat = lat;
    else if (lat > maxLat)
        maxLat = lat;
}

std::pair<double, double> Map::Dimensions::getCenter() const {

    const double lon = minLon + (maxLon - minLon) / 2;
    const double lat = minLat + (maxLat - minLat) / 2;

    return {lon, lat};
}