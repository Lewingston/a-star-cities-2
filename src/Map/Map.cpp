
#include "Map.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace asc2;

void Map::addNode(const Node& node) {

    const auto [iter, success] = nodes.emplace(node.id, node);
    if (!success) {
        std::cerr << "Failed to add node with id: " + std::to_string(node.id) +  " to map!\n";
    } else {
        dimensions.adjust(node.lon, node.lat);
    }
}

void Map::addWay(const Way& way) {

    const auto [iter, success] = ways.emplace(way.id, way);
    if (!success) {
        std::cerr << "Failed to add way with id: " + std::to_string(way.id) + " to map!\n";
    }
}

void Map::addRoad(RoadType type, uint64_t wayId) {

    if (auto find = ways.find(wayId); find != ways.end()) {
        roads.push_back(Road(type, find->second));
    } else {
        std::cerr << "Failed to add road with id: " << wayId << " - Way not found!\n";
    }
}

void Map::addBuilding(BuildingType type, uint64_t wayId) {

    if (auto find = ways.find(wayId); find != ways.end()) {
        buildings.push_back(Building(type, find->second));
    } else {
        std::cerr << "Failed to add building with id: " << wayId << " - Way not found!\n";
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