
#include "MapRenderer.h"
#include "../Map/Map.h"

using namespace asc2;

void MapRenderer::init(const Map& map) {

    //createBufferFromAllWays(map);
    createBufferFromRoads(map);
    createBufferFromBuildings(map);
}

void MapRenderer::draw(sf::RenderTarget& target) {

    if (lineBufferAllWays)
        lineBufferAllWays->draw(target);

    if (lineBufferRoads)
        lineBufferRoads->draw(target);

    if (lineBufferBuildings)
        lineBufferBuildings->draw(target);
}

void MapRenderer::createBufferFromAllWays(const Map& map) {

    lineBufferAllWays = std::make_unique<LineBuffer>(map.getAllWays());
}

void MapRenderer::createBufferFromRoads(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> roadWays;
    roadWays.reserve(map.getAllRoads().size());

    for (const Road& road : map.getAllRoads()) {
        roadWays.push_back(road.getWay());
    }

    lineBufferRoads = std::make_unique<LineBuffer>(roadWays);
}

void MapRenderer::createBufferFromBuildings(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> buildingWays;
    buildingWays.reserve(map.getAllBuildings().size());

    for (const Building& building : map.getAllBuildings()) {
        buildingWays.push_back(building.getWay());
    }

    lineBufferBuildings = std::make_unique<LineBuffer>(buildingWays);
}