
#include "MapParser.h"

#include "../Map/BuildingType.h"

#include <fstream>
#include <iostream>
#include <map>

using namespace asc2;

MapParser::MapParser(const std::string& filePath, const MapParserConfig& config) :
    config(config),
    wayParser(config) {

    loadFromFile(filePath);
}

void MapParser::loadFromFile(const std::string& filePath) {

    std::ifstream input(filePath);
    if (!input.is_open())
        throw std::runtime_error("Failed to open file: " + filePath);

    data = json::parse(input);

    std::cout << "Json file: " << filePath << " loaded.\n";
}

std::unique_ptr<Map> MapParser::parse() {

    if (!data.contains("elements"))
        throw std::runtime_error("Data does not contain any elements");

    if (!data["elements"].is_array())
        throw std::runtime_error("Data does not contain any elements");

    map = std::make_unique<Map>();

    try {

        parseElements(data);
        addNodesToMap();
        addWaysToMap();
        //parseWayTypes();
        //constructRelations();

    } catch (const std::exception& e) {
        std::cerr << "Json parser error:\n";
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Parser error");
    }

    return std::move(map);
}

void MapParser::parseElements(const json& data) {

    for (const auto& element: data["elements"]) {

        if (element["type"] == "node") {
            nodeParser.parse(element);
        } else if (element["type"] == "way") {
            wayParser.parse(element);
        } else if (element["type"] == "relation") {
            parseRelation(element);
        } else {
            std::cout << "Element with unknown type: " << element["type"] << '\n';
        }
    }

    std::cout << "Node count:     " << nodeParser.getNodeCount() << '\n';
    std::cout << "Way count:      " << wayParser.getWayCount()   << '\n';
    std::cout << "Relation count: " << relations.size() << '\n';
}

void MapParser::addNodesToMap() {

    std::cout << "Add nodes to map...\n";

    for (const auto& [id, node] : nodeParser.getNodes()) {

        map->addNode({
            .id  = id,
            .lon = node.lon,
            .lat = node.lat
        });
    }
}

void MapParser::addWaysToMap() {

    std::cout << "Add ways to map...\n";

    for (auto& [id, way] : wayParser.getWays()) {

        if (!config.mapFeatures.contains(way.type) && !way.isPartOfRelation)
            continue;

        const auto nodes = map->getNodes(way.nodeIds);
        way.isComplete = nodes.size() == way.nodeIds.size();

        if (!config.includeIncompleteWays && !way.isComplete)
            continue;

        map->addWay({
            .id         = way.id,
            .nodes      = nodes,
            .isComplete = way.isComplete
        });
    }
}

void MapParser::constructRelations() {

    std::cout << "Contruct relations\n";

    uint32_t completeCount = 0;
    uint32_t incompleteCount = 0;

    for (auto& [id, relation] : relations) {

        constructRelation(relation);

        relation.isComplete ? completeCount++ : incompleteCount++;
    }

    std::cout << "Number of complete relations:   " << completeCount   << '\n';
    std::cout << "Number of incomplete relations: " << incompleteCount << '\n';
}

void MapParser::parseRelation(const json& data) {

    const uint64_t id = data["id"];

    const Relation relation {
        .id   = id,
        .data = data
    };

    const auto [iter, success] = relations.emplace(id, relation);
    if (!success) {
        std::cerr << "Dupliacate relation ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump();
    }
}

void MapParser::constructRelation(Relation& relation) const {

    relation.isComplete = false;

    if (!relation.data.contains("members") || relation.data["members"].size() == 0) {
        std::cout << "Relation has no members: " << relation.data.dump() << '\n';
        return;
    }

    const std::size_t memberCount = relation.data["members"].size();

    std::vector<uint64_t> wayIds;
    wayIds.reserve(memberCount);

    std::vector<uint64_t> nodeIds;
    nodeIds.reserve(memberCount);

    std::vector<uint64_t> relationIds;
    relationIds.reserve(memberCount);

    for (const auto& element : relation.data["members"]) {
        const std::string type = element["type"];
        const uint64_t id = element["ref"];
        if (type == "node")
            nodeIds.emplace_back(id);
        else if (type == "way")
            wayIds.emplace_back(id);
        else if (type == "relation")
            relationIds.emplace_back(id);
        else
            std::cout << "Relation has unknown member type: " << type << '\n';
    }

    // TODO: Recursive relations
    if (relationIds.size() > 0)
        return;

    // check if all nodes where found
    if (nodeIds.size() != map->getNodes(nodeIds).size())
        return;

    // check if all ways where found
    const auto ways = map->getWays(wayIds);
    if (ways.size() != wayIds.size())
        return;

    // check if all ways are complete
    for (const auto& way : ways) {
        if (!way.get().isComplete)
            return;
    }

    relation.isComplete = true;
}

void MapParser::parseWayTypes() {

    std::cout << "Parse way types\n";

    for (const auto& [id, way] : wayParser.getWays()) {

        if (!way.isComplete)
            continue;

        if (wayIsHighway(way)) {
            parseHighway(way);
        } else if (wayIsBuilding(way)) {
            parseBuilding(way);
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

bool MapParser::wayIsBuilding(const Way& way) const {

    if (!way.data.contains("tags"))
        return false;

    return way.data["tags"].contains("building");
}

void MapParser::parseBuilding(const Way& way) {

    const BuildingType type(way.data["tags"]["building"].get<std::string>());

    map->addBuilding(type, way.id);
}