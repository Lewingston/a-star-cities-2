
#include "AStarCities.h"
#include "MapParser/MapParser.h"
#include "Map/Map.h"
#include "MapRenderer/Window.h"

#include <iostream>

using namespace asc2;

int main(int argc, const char** args) {

    std::unique_ptr<Map> map;

    MapParserConfig parserConfig {
        .mapFeatures = {
            MapFeatureType::BUILDING,
            MapFeatureType::HIGHWAY,
            MapFeatureType::RAILWAY,
        }
    };

    try {
        MapParser parser("../../maps/speyer.json", parserConfig);
        //MapParser parser("../../maps/bingen_umgebung.json");
        //MapParser parser("../../maps/budesheim.json");
        map = parser.parse();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    Window window(800, 600, ProgramInfo::name);
    window.renderMap(*map);
    window.show();

    return 0;
}