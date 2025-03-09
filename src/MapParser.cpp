
#include "MapParser.h"

//#include "Map/RoadType.h"
#include "Map/Road.h"

#include <fstream>
#include <iostream>
#include <map>

using namespace asc2;

MapParser::MapParser(const std::string& filePath) {

    loadFromFile(filePath);
}

void MapParser::loadFromFile(const std::string& filePath) {

    std::ifstream input(filePath);
    if (!input.is_open())
        throw std::runtime_error("Failed to open file: " + filePath);

    data = json::parse(input);
}

std::unique_ptr<Map> MapParser::parse() {

    if (!data.contains("elements"))
        throw std::runtime_error("Data does not contain any elements");

    if (!data["elements"].is_array())
        throw std::runtime_error("Data does not contain any elements");

    map = std::make_unique<Map>();

    try {

        parseElements(data);
        constructWays();
        parseWayTypes();

    } catch (const std::exception& e) {
        std::cerr << "Json parser error:\n";
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Parser error");
    }

    return std::move(map);
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

    map->setCenter(dimensions.getCenter());

    if (numberOfElementsWithNoType > 0) {
        std::cout << "Number of elements without type: " << numberOfElementsWithNoType << '\n';
    }

    if (numberOfElementsWithUnknownType > 0) {
        std::cout << "number of elements with unknown type: " << numberOfElementsWithUnknownType << '\n';
    }

    std::cout << "Node count: " << nodes.size() << '\n';
    std::cout << "Way count: " << ways.size() << '\n';

    std::cout << "Map center: " << map->getCenter().second << " / " << map->getCenter().first << '\n';
}

void MapParser::constructWays() {

    uint32_t completeCount = 0;
    uint32_t incompleteCount = 0;

    for (auto [id, way] : ways) {

        constructWay(way);

        way.isComplete ? completeCount++ : incompleteCount++;
    }

    std::cout << "Number of complete ways: " << completeCount << '\n';
    std::cout << "Number of incomplete ways: " << incompleteCount << '\n';
}

void MapParser::parseNode(const json& data) {

    const uint64_t id  = data["id"];
    const double   lon = data["lon"];
    const double   lat = data["lat"];

    map->addNode({
        .id  = id,
        .lon = lon,
        .lat = lat
    });

    dimensions.adjust(lon, lat);

    const auto [iter, success] = nodes.emplace(id, Node{id, data});
    if (!success) {
        std::cerr << "Duplicate node ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }
}

void MapParser::parseWay(const json& data) {

    const uint64_t id = data["id"];

    const Way way {
        .id   = id,
        .data = data
    };

    const auto [iter, success] = ways.emplace(id, way);
    if (!success) {
        std::cerr << "Duplicate way ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }
}

void MapParser::constructWay(Way& way) const {

    if (!way.data.contains("nodes") || way.data["nodes"].size() == 0) {
        std::cout << "Way has no nodes:\n" << way.data.dump() << '\n';
        return;
    }

    const auto nodes = map->getNodes(getIdArray(way.data["nodes"]));

    way.isComplete = nodes.size() == way.data["nodes"].size();

    map->addWay({
        .id         = way.id,
        .nodes      = nodes,
        .isComplete = way.isComplete
    });
}

void MapParser::parseWayTypes() {

    for (const auto& [id, way] : ways) {

        if (wayIsHighway(way)) {
            parseHighway(way);
        }
    }
}

bool MapParser::wayIsHighway(const Way& way) const {

    if (!way.data.contains("tags")) {
        return false;
    }

    return way.data["tags"].contains("highway");
}

void MapParser::parseHighway(const Way& way) {

    const RoadType type(way.data["tags"]["highway"].get<std::string>());

    map->addRoad(type, way.id);
}

std::vector<uint64_t> MapParser::getIdArray(const json& data) {

    if (!data.is_array()) {
        std::cerr << "Failed to get array of ids. Element is not an array!\n";
        return {};
    }

    std::vector<uint64_t> ids;
    ids.reserve(data.size());

    for (const auto& id : data) {
        if (!id.is_number_integer()) {
            std::cerr << "Failed to get array of ids. Array contains invalid elements.\n";
            return {};
        }
        ids.emplace_back(id.get<uint64_t>());
    }

    return ids;
}

/************************************************************/
/*                struct MapParser::Dimensions              */
/************************************************************/

void MapParser::Dimensions::adjust(double lon, double lat) {

    if (lon < minLon)
        minLon = lon;
    else if (lon > maxLon)
        maxLon = lon;

    if (lat < minLat)
        minLat = lat;
    else if (lat > maxLat)
        maxLat = lat;
}

std::pair<double, double> MapParser::Dimensions::getCenter() const {

    const double lon = minLon + (maxLon - minLon) / 2;
    const double lat = minLat + (maxLat - minLat) / 2;

    return {lon, lat};
}