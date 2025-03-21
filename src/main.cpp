
#include "AStarCities.h"
#include "MapParser/MapParser.h"
#include "Map/Map.h"
#include "MapRenderer/Window.h"

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

        std::string filePath = "../../maps/roads_only/new_york.json";
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

    std::cout << "Road count: " << map->getAllRoads().size() << '\n';
    std::cout << "Building count: " << map->getAllBuildings().size() << '\n';

    Window window(800, 600, ProgramInfo::name);
    window.renderMap(*map, renderConfig);
    window.show();

    return 0;
}