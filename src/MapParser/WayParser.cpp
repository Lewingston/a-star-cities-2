
#include "WayParser.h"
#include "ParserUtils.h"

#include <iostream>

using namespace asc2;

WayParser::WayParser(const MapParserConfig& config) :
    config(config) { }

bool WayParser::parse(const json& data) {

    const uint64_t id = data["id"];

    if (!data.contains("nodes") || data["nodes"].size() == 0) {
        std::cout << "Way has no nodes:\n" << data.dump() << '\n';
        return true;
    }

    const MapFeatureType type = getType(data);

    const auto [iter, success] = ways.emplace(id, Way {
        .id         = id,
        .type       = type,
        .data       = data,
        .nodeIds    = ParserUtils::getIdsFromArray(data["nodes"]),
        .isComplete = false
    });

    if (!success) {
        std::cerr << "Duplicate way ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }

    return success;
}

MapFeatureType WayParser::getType(const json& data) {

    // Assume that if a way has no tags, it is part on a relation
    // and there for has no type.
    if (!data.contains("tags"))
        return MapFeatureType::NO_TYPE;

    const auto tags = data["tags"];

    for (MapFeatureType type : config.mapFeatures) {
        if (tags.contains(MapFeatureType::getTypeName(type)))
            return type;
    }

    return MapFeatureType::UNKNOWN;
}