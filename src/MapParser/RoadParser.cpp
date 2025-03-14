
#include "RoadParser.h"

using namespace asc2;

using json = nlohmann::json;

RoadType RoadParser::getRoadType(const WayParser::Way& way) {

    const std::string& featureType = MapFeatureType::getTypeName(way.type);

    if (way.type != MapFeatureType::HIGHWAY)
        throw std::runtime_error("Unable to get road type. Way is not a road. Way is " + featureType);

    const json tags = way.data["tags"];

    if (!tags.contains(featureType))
        throw std::runtime_error("Unable to get road type. Way is not a road.");

    const RoadType roadType(tags[featureType].get<std::string>());

    return roadType;
}