
#include "Map.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <numeric>

using namespace asc2;

const Node& Map::addNode(const Node& node) {

    idHandler.updateUsedIds(node.id);
    const auto [iter, success] = nodes.emplace(node.id, node);
    if (!success) {
        std::cerr << "Failed to add node with id: " + std::to_string(node.id) +  " to map!\n";
    } else {
        dimensions.adjust(node.lon, node.lat);
    }

    return iter->second;
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

        addIntersectionsToEndOfRoad(road);
    }
}

void Map::addIntersectionsToEndOfRoad(Road& road) {

    if (road.getWay().isLoop())
        return;

    const uint64_t firstNodeId = road.getWay().getNodes().front().get().id;
    const uint64_t lastNodeId  = road.getWay().getNodes().back().get().id;

    const uint64_t firstInterId = road.getIntersections().size() > 0 ?
                                      road.getIntersections().front().get().getId() :
                                      std::numeric_limits<uint64_t>::max();

    const uint64_t lastInterId = road.getIntersections().size() > 0 ?
                                     road.getIntersections().back().get().getId() :
                                     std::numeric_limits<uint64_t>::max();

    if (firstNodeId != firstInterId) {

        const Node& node = road.getWay().getNodes().front();
        auto [inter, success] = intersections.insert({node.id, node});
        road.addIntersectionToFront(inter->second);
        inter->second.addRoad(road);
    }

    if (lastNodeId != lastInterId) {

        const Node& node = road.getWay().getNodes().back();
        auto [inter, success] = intersections.insert({node.id , node});
        road.addIntersection(inter->second);
        inter->second.addRoad(road);
    }
}

void Map::segmentRoads(float length) {

    length = length * 360.0f / 40'000'000.0f;

    mainRoadCount = roads.size();
    mainIntersections.reserve(intersections.size());

    for (const auto& [id, inter] : intersections)
        mainIntersections.emplace_back(inter);

    std::vector<Road::NewRoadData> newRoads;
    std::vector<std::reference_wrapper<Road>> removeRoads;

    for (auto& [id, road] : roads) {
        
        const auto _newRoads = segmentRoad(road, length);
        if (_newRoads.size() > 1) {
            for (const auto& road : _newRoads)
                newRoads.emplace_back(road);
            removeRoads.emplace_back(road);
        } else if (_newRoads.size() == 1) {
            std::cerr << "Segmenting one road into one road. Something went wrong!\n";
        }
    }

    for (Road& road : removeRoads) {
        removeRoadAndWay(road);
    }

    for (const auto& newRoad : newRoads) {
        addRoadAndWay(newRoad);
    }
}

std::vector<Road::NewRoadData> Map::segmentRoad(Road& road, float length) {

    const std::vector<Way::NewNode> newNodes = road.getWay().segmentate(length);
    if (newNodes.size() == 0)
        return {};

    std::vector<Road::NewRoadData> newRoads;
    newRoads.push_back(Road::NewRoadData {
        .type          = road.getType(), 
        .nodes         = {road.getWay().getNodes().front().get()},
        .intersections = {road.getIntersections().front()}
        });

    std::size_t index = 1;
    auto nextNewNode = newNodes.begin();

    for (auto nodeIter = road.getWay().getNodes().begin() + 1; nodeIter != road.getWay().getNodes().end(); nodeIter++) {


        while (nextNewNode != newNodes.end() && index == nextNewNode->index) {
            const Node&   node         = addNode(Node{idHandler.getNewId(), nextNewNode->lon, nextNewNode->lat});
            Intersection& intersection = intersections.insert({node.id, Intersection(node)}).first->second;

            newRoads.back().nodes.emplace_back(node);
            newRoads.back().intersections.emplace_back(intersection);

            nextNewNode = std::next(nextNewNode);

            newRoads.push_back(Road::NewRoadData {
                .type          = road.getType(),
                .nodes         = { node },
                .intersections = { intersection }
            });

            index++;
        }

        newRoads.back().nodes.emplace_back(nodeIter->get());
        index++;
    }

    newRoads.back().intersections.emplace_back(road.getIntersections().back());

    /*for (const auto& newNode : newNodes) {

        const Node& node = addNode(Node{idHandler.getNewId(), newNode.lon, newNode.lat});
        intersections.insert({node.id, Intersection(node)});
    }*/

    return newRoads;
}

std::size_t Map::removeRoadsWithoutIntersections() {

    std::vector<std::reference_wrapper<Road>> removeRoads;

    for (auto& [id, road] : roads) {

        if (road.getIntersections().size() == 0)
            removeRoads.emplace_back(road);
    }

    for (Road& road : removeRoads) {
        removeRoadAndWay(road);
    }

    std::cout << "Removed " << removeRoads.size() << " roads without intersection.\n";

    return removeRoads.size();
}

void Map::removeNetwork(const NetworkFinder::Network& network) {

    for (const auto& [id, road] : network.roads) {
        roads.erase(id);
        ways.erase(id);
    }

    for (const auto& [id, intersection] : network.intersections) {
        intersections.erase(id);
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

std::pair<double, double> Map::getPosition() const {

    return dimensions.getPos();
}

std::pair<double, double> Map::getSize() const {

    return dimensions.getSize();
}

float Map::getTotalRoadLength() const {

    const float length = std::accumulate(roads.begin(), roads.end(), 0.0f,
        [](float length, const auto& roadMapEntry) -> float {
            return length + roadMapEntry.second.getLength();
        });

    return length * 40'000'000.0f / 360.0f;
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

std::pair<double, double> Map::Dimensions::getPos() const {

    return {minLon, minLat};
}

std::pair<double, double> Map::Dimensions::getSize() const {

    return {maxLon - minLon, maxLat - minLat};
}