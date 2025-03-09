
#include "Map.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace asc2;

void Map::addNode(const Node& node) {

    const auto [iter, success] = nodes.emplace(node.id, node);
    if (!success) {
        std::cerr << "Failed to add node with id: " + std::to_string(node.id) +  " to map!\n";
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
        std::cerr << "Failed to add road with id: " << wayId << " Way not found!\n";
    }
}

std::vector<std::reference_wrapper<const Node>> Map::getNodes(
    const std::vector<uint64_t>& nodeIds) {

    std::vector<std::reference_wrapper<const Node>> nodes;
    nodes.reserve(nodeIds.size());

    for (uint64_t id : nodeIds) {

        if (auto find = this->nodes.find(id); find != this->nodes.end()) {
            nodes.emplace_back(find->second);
        }
    }

    return nodes;
}

const std::map<uint64_t, Node>& Map::getAllNodes() const noexcept {

    return nodes;
}

const std::map<uint64_t, Way>& Map::getAllWays() const noexcept {

    return ways;
}