
#include "AStarCities.h"
#include "MapParser/MapParserConfig.h"
#include "MapParser/MapParser.h"
#include "MapLoader/MapLoader.h"
#include "AStar/AStarWindow.h"

#include <iostream>

using namespace asc2;

const bool enableIncomplete = false;

const MapParserConfig defaultParserConfig {
    .mapFeatures = {
        MapFeatureType::NO_TYPE,
        MapFeatureType::UNKNOWN,
        //MapFeatureType::BUILDING,
        //MapFeatureType::HIGHWAY
    },
    .includeIncompleteWays = enableIncomplete
};

const MapParserConfig aStarParserConfig {
    .mapFeatures = {
        MapFeatureType::NO_TYPE,
        MapFeatureType::UNKNOWN,
        MapFeatureType::HIGHWAY
    }
};

const RenderConfig defaultRenderConfig {
    .displayIncomplete = enableIncomplete,
    .renderAllLines    = true,
    //.renderRoads       = true,
    //.renderBuildings   = true,
};

const RenderConfig aStarRenderConfig {
    .displayIncomplete   = enableIncomplete,
    .buildingLineMode    = false,
    .randomColors        = false,
    .renderAllLines      = false,
    .renderRoads         = true,
    .renderIntersections = false,
    .backgroundColor     = sf::Color::Black,
    .roadColor           = sf::Color::Black,
    .roadColors          = {
        { RoadType::LIVING_STREET,  sf::Color(16, 16, 16) },
        { RoadType::RESIDENTIAL,    sf::Color(18, 18, 18) },
        { RoadType::UNCLASSIFIED,   sf::Color(20, 20, 20) },
        { RoadType::TERTIARY,       sf::Color(22, 22, 22) },
        { RoadType::TERTIARY_LINK,  sf::Color(22, 22, 22) },
        { RoadType::SECONDARY,      sf::Color(24, 24, 24) },
        { RoadType::SECONDARY_LINK, sf::Color(24, 24, 24) },
        { RoadType::PRIMARY,        sf::Color(26, 26, 26) },
        { RoadType::PRIMARY_LINK,   sf::Color(26, 26, 26) },
        { RoadType::TRUNK,          sf::Color(28, 28, 28) },
        { RoadType::TRUNK_LINK,     sf::Color(28, 28, 28) },
        { RoadType::MOTORWAY,       sf::Color(30, 30, 30) },
        { RoadType::MOTORWAY_LINK,  sf::Color(30, 30, 30) }
    }
};

std::shared_ptr<Map> loadMapWithoutWindow(const std::string& filePath);
void loadMapWithWindow(const std::string& filePath, Window& window);
std::shared_ptr<Map> loadMapWithoutWindowPrepareForAStar(const std::string& filePath);
void loadMapWithWindowPrepareForAStar(const std::string& filePath, Window& window, const RenderConfig& renderConfig);

int main(int argc, const char** args) {

    //std::string filePath = "../../maps/roads_only/new_york.json";
    std::string filePath;

    std::string mode = "normal";

    if (argc == 2) {
        filePath = std::string(args[1]);
    } else if (argc == 3) {
        mode = std::string(args[1]);
        filePath = std::string(args[2]);
    }

    if (mode == "normal") {
        AStarWindow window(1600, 900, ProgramInfo::name, false);
        loadMapWithWindowPrepareForAStar(filePath, window, aStarRenderConfig);
    } else if (mode == "interactive") {
        AStarWindow window(1600, 900, ProgramInfo::name, true);
        loadMapWithWindowPrepareForAStar(filePath, window, aStarRenderConfig);
    } else {
        std::cout << "Available modes:\n";
        std::cout << "    normal\n    interactive\n";
        return 1;
    }

    return 0;
}

std::shared_ptr<Map> loadMapWithoutWindow(const std::string& filePath) {

    MapParser parser(filePath, defaultParserConfig);

    std::shared_ptr<Map> map = parser.parse();
    if (map == nullptr) {
        std::cerr << "Failed to parse map: " << filePath << '\n';
    }

    return map;
}

void loadMapWithWindow(const std::string& filePath, Window& window) {

    MapLoader mapLoader(filePath, defaultParserConfig);
    mapLoader.setDefualtLoad();

    window.show(mapLoader, defaultRenderConfig);
}

std::shared_ptr<Map> loadMapWithoutWindowPrepareForAStar(const std::string& filePath) {

    MapParser parser(filePath, aStarParserConfig);

    std::shared_ptr<Map> map = parser.parse();
    if (map == nullptr) {
        std::cerr << "Failed to parse map: " << filePath << '\n';
    }

    map->optimizeIntersecions();

    NetworkFinder networkFinder(*map);
    networkFinder.findNetworks();
    networkFinder.removeUnconnectedNetworks();

    map->addIntersectionsToEndPoints();

    map->segmentRoads(50.0f);

    return map;
}

void loadMapWithWindowPrepareForAStar(const std::string& filePath, Window& window, const RenderConfig& renderConfig) {

    MapLoader mapLoader(filePath, aStarParserConfig);
    mapLoader.setAStarLoad();

    window.show(mapLoader, renderConfig);
}