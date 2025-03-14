
#include "BuildingParser.h"

#include <iostream>

using namespace asc2;

BuildingType BuildingParser::getBuildingType(const WayParser::Way& way) {

    const std::string featureType = MapFeatureType::getTypeName(way.type);

    if (way.type != MapFeatureType::BUILDING)
        throw std::runtime_error("Unable to get building type. Way is not a building. Way is " + featureType);

    return getBuildingType(way.data);
}

BuildingType BuildingParser::getBuildingType(const RelationParser::Relation& relation) {

    const std::string featureType = MapFeatureType::getTypeName(relation.type);

    if (relation.type != MapFeatureType::BUILDING)
        throw std::runtime_error("Unable to get building type. Relation is not a building. Way is " + featureType);

    return getBuildingType(relation.data);
}

BuildingType BuildingParser::getBuildingType(const json& data) {

    static const std::string featureType = MapFeatureType::getTypeName(MapFeatureType::BUILDING);

    const json tags = data["tags"];

    if (!tags.contains(featureType)) {
        throw std::runtime_error("Unable to get building type. Element is not a building.");
    }

    const BuildingType buildingType(tags[featureType].get<std::string>());

    return buildingType;
}