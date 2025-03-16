
#include "MapParser.h"
#include "RoadParser.h"
#include "BuildingParser.h"

#include "../Map/BuildingType.h"
#include "../Map/MapFeatureType.h"

#include <fstream>
#include <iostream>
#include <map>

using namespace asc2;

MapParser::MapParser(const std::string& filePath, const MapParserConfig& config) :
    config(config),
    wayParser(config),
    relationParser(config) {

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

        if (!parseElements(data)) {
            return nullptr;
        }

        relationParser.constructRelations(wayParser.getWays());

        addNodesToMap();
        addWaysToMap();

        parseWayTypes();
        parseRelationTypes();

    } catch (const std::exception& e) {
        std::cerr << "Json parser error:\n";
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Parser error");
    }

    return std::move(map);
}

bool MapParser::parseElements(const json& data) {

    for (const auto& element: data["elements"]) {

        if (!parseElement(element)) {
            std::cerr << "Failed to parse element:\n";
            std::cerr << element.dump();
            return false;
        }
    }

    std::cout << "Node count:     " << nodeParser.getNodeCount() << '\n';
    std::cout << "Way count:      " << wayParser.getWayCount() << '\n';
    std::cout << "Relation count: " << relationParser.getRelationCount() << '\n';

    return true;
}

bool MapParser::parseElement(const json& element) {

    if (element["type"] == "node") {
        return nodeParser.parse(element);
    } else if (element["type"] == "way") {
        return wayParser.parse(element);
    } else if (element["type"] == "relation") {
        return relationParser.parse(element);
    } else {
        std::cout << "Element with unknown type: " << element["type"] << '\n';
        return true;
    }
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

        if (!config.includeIncompleteWays && !way.isComplete && !way.isPartOfRelation)
            continue;

        map->addWay({
            .id         = way.id,
            .nodes      = nodes,
            .isComplete = way.isComplete
        });
    }
}

void MapParser::parseWayTypes() {

    std::cout << "Parse way types\n";

    for (const auto& [id, way] : wayParser.getWays()) {

        if (!way.isComplete && !config.includeIncompleteWays)
            continue;

        if (!config.mapFeatures.contains(way.type))
            continue;

        switch (way.type.getEnumValue()) {
            case MapFeatureType::HIGHWAY:
                map->addRoad(RoadParser::getRoadType(way), way.id);
                break;
            case MapFeatureType::BUILDING:
                map->addBuilding(BuildingParser::getBuildingType(way), way.id);
                break;
            default:
                break;
        }
    }
}

void MapParser::parseRelationTypes() {

    std::cout << "Parse relation types\n";

    for (const auto& [id, relation] : relationParser.getRelations()) {

        if (!relation.isComplete)
            continue;

        if (!config.mapFeatures.contains(relation.type))
            continue;

        switch (relation.type.getEnumValue()) {
            case MapFeatureType::BUILDING:
                addBuilding(relation);
                break;
            default:
                break;
        }
    }
}

void MapParser::addBuilding(const RelationParser::Relation& relation) {

    std::vector<uint64_t> outerWays(relation.outerWays.size());
    std::vector<uint64_t> innerWays(relation.innerWays.size());

    for (std::size_t ii = 0; ii < relation.outerWays.size(); ii++)
        outerWays[ii] = relation.outerWays[ii].get().id;

    for (std::size_t ii = 0; ii < relation.innerWays.size(); ii++)
        innerWays[ii] = relation .innerWays[ii].get().id;

    map->addBuilding(BuildingParser::getBuildingType(relation), relation.id, outerWays, innerWays);
}