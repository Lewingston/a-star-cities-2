
/*#include "AStarCities.h"
#include "MapParser/MapParser.h"
#include "Map/Map.h"
#include "MapRenderer/Window.h"
#include "MapUtilities/NetworkFinder.h"*/

//#include <iostream>

#include "AStarCities.h"
#include "AStar/AStarWindow.h"

#include <iostream>

using namespace asc2;

int main(int argc, const char** args) {

    /*std::unique_ptr<Map> map;

    const bool enableIncomplete = false;

    MapParserConfig parserConfig {
        .mapFeatures = {
            //MapFeatureType::NO_TYPE,
            //MapFeatureType::UNKNOWN,
            //MapFeatureType::BUILDING,
            MapFeatureType::HIGHWAY,
            //MapFeatureType::RAILWAY,
        },
        .includeIncompleteWays = enableIncomplete
    };

    RenderConfig renderConfig {
        .displayIncomplete = enableIncomplete,
        .buildingLineMode  = false,
        .backgroundColor   = sf::Color::Black,
        .roadColor         = sf::Color(80, 0, 0),
        .roadColors        = {
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

    try {

        std::string filePath = "../../maps/roads_only/speyer.json";
        if (argc == 2)
            filePath = std::string(args[1]);

        MapParser parser(filePath, parserConfig);

        map = parser.parse();
        if (map == nullptr) {
            std::cerr << "Failed to parse map: " << filePath << '\n';
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    map->optimizeIntersecions();

    NetworkFinder networkFinder(*map);
    networkFinder.findNetworks();
    networkFinder.removeUnconnectedNetworks();

    map->addIntersectionsToEndPoints();

    map->segmentRoads(50.0f);

    const float totalRoadLength = map->getTotalRoadLength();
    const float averageRoadLength = totalRoadLength / static_cast<float>(map->getAllRoads().size());
    std::cout << "Total road length: " << totalRoadLength << "m\n";
    std::cout << "Average road length: " << averageRoadLength << "m\n";

    std::cout << "Road count: " << map->getAllRoads().size() << '\n';
    std::cout << "Building count: " << map->getAllBuildings().size() << '\n';

    Window window(1600, 900, ProgramInfo::name);
    window.renderMap(*map, renderConfig);
    window.show();*/

    const std::string filePath = argc == 2 ?
        std::string(args[1]) :
        "../../maps/roads_only/speyer.json";

    try {
        AStarWindow window(1600, 900, ProgramInfo::name);
        window.setMapFile(filePath);
        window.show();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}