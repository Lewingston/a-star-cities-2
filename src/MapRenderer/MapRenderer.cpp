
#include "MapRenderer.h"
#include "ShapeRenderer.h"
#include "NodeRenderer.h"
#include "LineBuffer.h"
#include "ShapeBuffer.h"
#include "../Map/Map.h"

#include <iostream>
#include <numeric>

using namespace asc2;

void MapRenderer::init(const Map& map,
                       const RenderConfig& config) {

    this->config = config;

    //createBufferFromAllWays(map);
    createLineBufferFromRoads(map);
    //createLineBufferFromBuildings(map);

    //createShapeBufferFromBuildings(map);

    createNodeBufferFromIntersections(map);

    std::size_t vertexCount = 0;
    std::size_t edgeCount = 0;
    std::size_t polygonCount = 0;

    for (const auto& buffer : renderBuffers) {
        vertexCount  += buffer->getVertexCount();
        edgeCount    += buffer->getLineCount();
        polygonCount += buffer->getPolygonCount();
    }

    std::cout << "Total vertices: " << vertexCount << '\n';
    std::cout << "Total edges: " << edgeCount << '\n';
    std::cout << "Total polygon count: " << polygonCount << '\n';
}

void MapRenderer::draw(sf::RenderTarget& target) {

    for (const auto& renderBuffer : renderBuffers) {

        renderBuffer->draw(target);
    }
}

void MapRenderer::createBufferFromAllWays(const Map& map) {

    auto buffer = std::make_unique<LineBuffer>(map.getAllWays(), config);
    renderBuffers.push_back(std::move(buffer));
}

void MapRenderer::createLineBufferFromRoads(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> roadWays;
    roadWays.reserve(map.getAllRoads().size());

    //for (const Road& road : map.getAllRoads()) {
    for (const auto& [id, road] : map.getAllRoads()) {
        roadWays.push_back(road.getWay());
    }

    auto buffer = std::make_unique<LineBuffer>(roadWays, config);
    renderBuffers.push_back(std::move(buffer));
}

void MapRenderer::createLineBufferFromBuildings(const Map& map) {

    std::vector<std::reference_wrapper<const Way>> buildingWays;
    buildingWays.reserve(getBuildingWayCount(map));

    for (const Building& building : map.getAllBuildings()) {

        const auto& outerWays = building.getOuterWays();
        const auto& innerWays = building.getInnerWays();

        buildingWays.insert(buildingWays.end(), outerWays.begin(), outerWays.end());
        buildingWays.insert(buildingWays.end(), innerWays.begin(), innerWays.end());
    }

    auto buffer = std::make_unique<LineBuffer>(buildingWays, config);
    renderBuffers.push_back(std::move(buffer));
}

void MapRenderer::createShapeBufferFromBuildings(const Map& map) {

    std::vector<ShapeRenderer> shapes;
    shapes.reserve(map.getAllBuildings().size());

    for (const Building& building : map.getAllBuildings()) {

        shapes.emplace_back(building.getOuterWays(), building.getInnerWays());
    }

    auto buffer = std::make_unique<ShapeBuffer>(shapes, config.buildingLineMode);
    renderBuffers.push_back(std::move(buffer));
}

std::size_t MapRenderer::getBuildingWayCount(const Map& map) const {

    const auto& buildings = map.getAllBuildings();

    const std::size_t count = std::accumulate(buildings.begin(), buildings.end(), 0u,
        [](std::size_t count, const Building& building) -> std::size_t {
            return count + building.getOuterWays().size() + building.getInnerWays().size();
        });

    return count;
}

void MapRenderer::createNodeBufferFromIntersections(const Map& map) {

    std::vector<NodeRenderer> nodes;
    nodes.reserve(map.getAllIntersections().size());

    for (const auto& [id, intersection] : map.getAllIntersections()) {

        /*if (intersection.getRoads().size() < 2)
            continue;*/

        nodes.emplace_back(intersection.getNode().lon, intersection.getNode().lat);
    }

    auto buffer = std::make_unique<ShapeBuffer>(nodes);
    renderBuffers.push_back(std::move(buffer));
}