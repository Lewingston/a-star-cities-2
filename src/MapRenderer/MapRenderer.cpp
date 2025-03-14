
#include "MapRenderer.h"
#include "../Map/Map.h"

using namespace asc2;

void MapRenderer::init(const Map& map,
                       const RenderConfig& config) {

    this->config = config;

    //createBufferFromAllWays(map);
    //createLineBufferFromRoads(map);
    createLineBufferFromBuildings(map);

    //createShapeBufferFromBuildings(map);
}

void MapRenderer::draw(sf::RenderTarget& target) {

    if (shapeBufferBuildings)
        shapeBufferBuildings->draw(target);

    if (lineBufferAllWays)
        lineBufferAllWays->draw(target);

    if (lineBufferRoads)
        lineBufferRoads->draw(target);

    if (lineBufferBuildings)
        lineBufferBuildings->draw(target);
}

void MapRenderer::createBufferFromAllWays(const Map& map) {

    lineBufferAllWays = std::make_unique<LineBuffer>(map.getAllWays(), config);
}

void MapRenderer::createLineBufferFromRoads(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> roadWays;
    roadWays.reserve(map.getAllRoads().size());

    for (const Road& road : map.getAllRoads()) {
        roadWays.push_back(road.getWay());
    }

    lineBufferRoads = std::make_unique<LineBuffer>(roadWays, config);
}

void MapRenderer::createLineBufferFromBuildings(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> buildingWays;
    buildingWays.reserve(map.getAllBuildings().size());

    for (const Building& building : map.getAllBuildings()) {

        for (const Way& way : building.getOuterWays())
            buildingWays.push_back(way);

        for (const Way& way : building.getInnerWays())
            buildingWays.push_back(way);
    }

    lineBufferBuildings = std::make_unique<LineBuffer>(buildingWays, config);
}

void MapRenderer::createShapeBufferFromBuildings(const Map& map) {

}