
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

    } catch (const std::exception& e) {
        std::cerr << "Json parser error:\n";
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Parser error");
    }

    /*std::size_t totalElementCount = 0;
    std::size_t noTypeElementCount = 0;
    std::map<std::string, std::size_t> elementTypeMap;

    for (const auto& element: data["elements"]) {

        totalElementCount += 1;

        if (!element.contains("type")) {
            noTypeElementCount += 1;
            continue;
        }

        if (auto find = elementTypeMap.find(element["type"]); find != elementTypeMap.end()) {
            find->second++;
        } else {
            elementTypeMap.emplace(element["type"], 1);
        }
    }

    std::vector<std::pair<std::string, std::size_t>> elementCounts;
    elementCounts.reserve(elementTypeMap.size());
    for (const auto& element : elementTypeMap)
        elementCounts.emplace_back(element);

    std::sort(elementCounts.begin(), elementCounts.end(), [](auto a, auto b) {
        return a.second > b.second;
    });

    std::cout << "Total Element Count: " << totalElementCount << '\n';

    for (const auto& element : elementCounts)
        std::cout << element.first << ": " << element.second << '\n';

    std::cout << "Elements with no type: " << noTypeElementCount << '\n';*/
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

    parseWays();

    std::cout << "Node count: " << nodes.size() << '\n';
    std::cout << "Way count: " << ways.size() << '\n';
}

void MapParser::parseWays() {

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