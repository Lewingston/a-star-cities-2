
#include "AStarCities.h"
#include "MapParser.h"
#include "Projector.h"
#include "Map/Map.h"
#include "MapRenderer/Window.h"

#include <iostream>

using namespace asc2;

int main(int argc, const char** args) {

    std::unique_ptr<Map> map;

    try {
        MapParser parser("../../maps/budesheim.json");
        map = parser.parse();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    Projector::project(*map);

    Window window(800, 600, ProgramInfo::name);
    window.renderMap(*map);
    window.show();

    return 0;
}