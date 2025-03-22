
#include "AStarCities.h"
#include "MapParser/MapParser.h"
#include "Map/Map.h"
#include "MapRenderer/Window.h"
#include "MapUtilities/NetworkFinder.h"

#include <iostream>

using namespace asc2;

int main(int argc, const char** args) {

    std::unique_ptr<Map> map;

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
        .buildingLineMode  = false
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

    Window window(800, 600, ProgramInfo::name);
    window.renderMap(*map, renderConfig);
    window.show();

    return 0;
}