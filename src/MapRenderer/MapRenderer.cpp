
#include "MapRenderer.h"
#include "ShapeRenderer.h"
#include "NodeRenderer.h"
#include "LineRenderer.h"
#include "LineBuffer.h"
#include "ShapeBuffer.h"
#include "Color.h"
#include "../Map/Map.h"
#include "../MapUtilities/NetworkFinder.h"

#include <iostream>
#include <numeric>

using namespace asc2;

void MapRenderer::init(const Map& map,
                       const RenderConfig& config) {

    this->config = config;

    if (config.renderAllLines)
        createBufferFromAllWays(map);

    if (config.renderRoads)
        createLineBufferFromRoads(map);

    if (config.renderBuildings) {
        if (config.buildingLineMode)
            createLineBufferFromBuildings(map);
        else
            createShapeBufferFromBuildings(map);
    }

    if (config.renderIntersections)
        createNodeBufferFromIntersections(map);

    //createBuffersForRoadNetworks(map);

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

    std::vector<LineRenderer> lines;
    lines.reserve(map.getAllRoads().size());

    for (const auto& [id, road] : map.getAllRoads()) {

        const sf::Color color = [&]() -> sf::Color {

            if (config.randomColors)
                return Color::getRandomColor();

            auto find = config.roadColors.find(road.getType());
            if (find != config.roadColors.end())
                return find->second;
            else
                return config.roadColor;
        }();

        lines.emplace_back(road.getWay(), color);
    }

    auto buffer = std::make_unique<LineBuffer>(lines, config);
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

    //const sf::Color color = Color::getRandomColor();

    for (const auto& [id, intersection] : map.getAllIntersections()) {

        const sf::Color color = Color::getRandomColor();
        nodes.emplace_back(intersection.getNode().lon, intersection.getNode().lat, color);
    }

    auto buffer = std::make_unique<ShapeBuffer>(nodes);
    renderBuffers.push_back(std::move(buffer));
}

void MapRenderer::createBuffersForRoadNetworks(const Map& map) {

    const auto networks = NetworkFinder::findNetworks(map);

    std::vector<NodeRenderer> nodes;
    nodes.reserve(map.getAllIntersections().size());

    std::vector<LineRenderer> lines;
    lines.reserve(map.getAllRoads().size());

    for (const auto& network : networks) {

        const sf::Color nodeColor = Color::getRandomColor();
        const sf::Color roadColor = Color::getRandomColor();

        for (const auto& [id, intersection] : network.intersections) {
            nodes.emplace_back(intersection.get().getNode().lon, intersection.get().getNode().lat, nodeColor);
        }

        for (const auto& [id, road] : network.roads) {
            lines.emplace_back(road.get().getWay(), roadColor);
        }
    }

    auto lineBuffer = std::make_unique<LineBuffer>(lines, config);
    renderBuffers.push_back(std::move(lineBuffer));

    auto nodeBuffer = std::make_unique<ShapeBuffer>(nodes);
    renderBuffers.push_back(std::move(nodeBuffer));
}