
#include "MapParser.h"

#include <fstream>
#include <iostream>
#include <map>

using namespace asc2;

void MapParser::loadFromFile(const std::string& filePath) {

    std::ifstream input(filePath);
    if (!input.is_open())
        throw std::runtime_error("Failed to open file: " + filePath);

    data = json::parse(input);
}

void MapParser::parse() {

    if (!data.contains("elements"))
        throw std::runtime_error("Data does not contain any elements");

    if (!data["elements"].is_array())
        throw std::runtime_error("Data does not contain any elements");

    try {

        parseElements(data);
        constructWays();

    } catch (const std::exception& e) {
        std::cerr << "Json parser error:\n";
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Parser error");
    }
}

void MapParser::parseElements(const json& data) {

    std::size_t numberOfElementsWithNoType = 0;
    std::size_t numberOfElementsWithUnknownType = 0;

    for (const auto& element: data["elements"]) {

        if (!element.contains("type")) {
            numberOfElementsWithNoType = 0;
            continue;
        }

        if (element["type"] == "node") {
            parseNode(element);
        } else if (element["type"] == "way") {
            parseWay(element);
        } else if (element["type"] == "relation") {
            // TODO: ???
        }
    }

    if (numberOfElementsWithNoType > 0) {
        std::cout << "Number of elements without type: " << numberOfElementsWithNoType << '\n';
    }

    if (numberOfElementsWithUnknownType > 0) {
        std::cout << "number of elements with unknown type: " << numberOfElementsWithUnknownType << '\n';
    }

    std::cout << "Node count: " << nodes.size() << '\n';
    std::cout << "Way count: " << ways.size() << '\n';
}

void MapParser::constructWays() {

    uint32_t completeCount = 0;
    uint32_t incompleteCount = 0;

    for (Way& way : ways) {
        constructWay(way);

        way.isComplete ? completeCount++ : incompleteCount++;
    }

    std::cout << "Number of complete ways: " << completeCount << '\n';
    std::cout << "Number of incomplete ways: " << incompleteCount << '\n';
}

void MapParser::parseNode(const json& data) {

    const uint64_t id = data["id"];
    const double lon  = data["lon"];
    const double lat  = data["lat"];

    const Node node {
        .id   = id,
        .lon  = lon,
        .lat  = lat,
        .data = data
    };

    const auto [iter, success] = nodes.emplace(id, node);
    if (!success) {
        std::cerr << "Duplicate nodes: \n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << node.data.dump() << '\n';
    }
}

void MapParser::parseWay(const json& data) {

    const uint64_t id = data["id"];

    const Way way {
        .id    = id,
        .nodes = {},
        .data  = data
    };

    ways.emplace_back(way);
}

void MapParser::constructWay(Way& way) const {

    if (!way.data.contains("nodes") || way.data["nodes"].size() == 0) {
        std::cout << "Way has no nodes:\n" << way.data.dump() << '\n';
        return;
    }

    way.isComplete = true;
    way.nodes.clear();
    way.nodes.reserve(way.data["nodes"].size());

    for (const auto& node : way.data["nodes"]) {

        const uint64_t nodeId = node.get<uint64_t>();

        if (auto find = nodes.find(nodeId); find != nodes.end()) {
            way.nodes.emplace_back(find->second);
        } else {
            way.isComplete = false;
        }
    }
}