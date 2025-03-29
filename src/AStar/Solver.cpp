
#include "Solver.h"
#include "BorderDrawer.h"
#include "../MapUtilities/Geometry.h"

#include <SFML/Graphics/CircleShape.hpp>

#include <cmath>

using namespace asc2;

Solver::Solver(AStarOverlay& overlay, std::shared_ptr<Map> map) :
    overlay(overlay),
    map(map),
    randomGenerator(std::random_device{}()) { }

void Solver::selectStartAndEndNode() {

    const sf::Vector2f overlaySize = overlay.getSize();

    const std::vector<std::reference_wrapper<const Intersection>> intersections = getIntersectionsInArea(
        overlay.getPosition(), overlaySize, overlay.getRotation());

    const float minDistance = overlaySize.length() / 2.0f;

    const auto& [inter1, inter2] = selectRandomIntersections(intersections, minDistance);

    startPoint = &inter1;
    endPoint   = &inter2;

    drawStartAndEndPoint(overlay.getCurrentTexture());
}

std::vector<std::reference_wrapper<const Intersection>> Solver::getIntersectionsInArea(sf::Vector2f center, sf::Vector2f size, float rotation) const {

    BorderDrawer borders;
    borders.update(center, size, rotation);
    return getIntersectionsInArea(borders.getPoints());
}

std::vector<std::reference_wrapper<const Intersection>> Solver::getIntersectionsInArea(const std::array<sf::Vector2f, 4>& area) const {

    std::vector<std::reference_wrapper<const Intersection>> inArea;

    if (!map)
        return inArea;

    const Geometry::Rectangle rectangle = { area };

    for (const Intersection& inter : map->getMainIntersections()) {

        const sf::Vector2f pos {
            static_cast<float>(inter.getNode().lon),
            static_cast<float>(-inter.getNode().lat)
        };

        if (Geometry::pointInRectangle(pos, rectangle)) {
            inArea.emplace_back(inter);
        }
    }

    return inArea;
}

const Intersection& Solver::selectRandomIntersection(const std::vector<std::reference_wrapper<const Intersection>>& intersections) {

    std::uniform_int_distribution<std::size_t> distribution(0, intersections.size() - 1);
    std::size_t index = distribution(randomGenerator);

    return intersections.at(index);
}

std::pair<const Intersection&, const Intersection&> Solver::selectRandomIntersections(
    const std::vector<std::reference_wrapper<const Intersection>>& intersections,
    float distance) {

    uint8_t tryCount = 0;
    uint8_t totalTryCount = 0;

    while (totalTryCount < 100) {

        const Intersection& inter1 = selectRandomIntersection(intersections);
        const Intersection& inter2 = selectRandomIntersection(intersections);

        if (inter1 == inter2)
            continue;

        if (inter1.getNode().distance(inter2.getNode()) > distance) {
            return {inter1, inter2};
        }

        tryCount++;
        if (tryCount >= 1) {
            tryCount = 0;
            distance *= 0.95f;
        }
        
        totalTryCount++;
    }

    return {
        selectRandomIntersection(intersections),
        selectRandomIntersection(intersections)
    };
}

void Solver::drawPoint(const Intersection& intersection, sf::RenderTarget& target) {

    const uint8_t radiusInPixels = 3;

    const sf::Vector2f size = overlay.getSize();
    const sf::Vector2f res = {
        static_cast<float>(overlay.getResolution().x),
        static_cast<float>(overlay.getResolution().y)
    };

    const float radius = size.x / res.x * static_cast<float>(radiusInPixels);

    //const float radius = overlay.getSize().length() * 0.002f;

    sf::CircleShape shape(radius);
    shape.setFillColor(color);
    shape.setPosition(sf::Vector2f(
        static_cast<float>(intersection.getNode().lon) - radius,
        static_cast<float>(-intersection.getNode().lat) - radius
    ));

    target.draw(shape);
}

void Solver::drawStartAndEndPoint(sf::RenderTarget& target) {

    if (startPoint)
        drawPoint(*startPoint, target);
    if (endPoint)
        drawPoint(*endPoint, target);
}