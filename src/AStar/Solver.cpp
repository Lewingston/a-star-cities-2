
#include "Solver.h"
#include "BorderDrawer.h"
#include "../MapUtilities/Geometry.h"
#include "../MapRenderer/LineRenderer.h"
#include "../MapRenderer/LineBuffer.h"

#include <SFML/Graphics/CircleShape.hpp>

#include <cmath>

using namespace asc2;

Solver::Solver(AStarOverlay& overlay, std::shared_ptr<Map> map) :
    overlay(overlay),
    map(map),
    randomGenerator(std::random_device{}()) { 

    if (map)
        initNodes();
}

void Solver::setMap(std::shared_ptr<Map> map) {

    this->map = map;

    if (map)
        initNodes();
    else
        nodes.clear();
}

void Solver::initNodes() {

    for (const auto& [id, intersection] : map->getAllIntersections()) {

        nodes.emplace(id, PathNode(intersection));
    }
}

void Solver::reset() {

    selectStartAndEndNode();
}

void Solver::restart() {

    openList.clear();
    closedList.clear();

    selectCurrentNode(startPoint->getId());

    currentNode->setDistanceTraveled(0);
    currentNode->setDistanceToTarget(startPoint->getNode().distance(endPoint->getNode()));
    openList.insert(*currentNode);

    solved = false;

    overlay.clear();
    drawStartAndEndPoint(overlay.getCurrentTexture());
}

void Solver::selectStartAndEndNode() {

    overlay.clear();

    const sf::Vector2f overlaySize = overlay.getSize();

    const std::vector<std::reference_wrapper<const Intersection>> intersections = getIntersectionsInArea(
        overlay.getPosition(), overlaySize, overlay.getRotation());

    const float minDistance = overlaySize.length() / 2.0f;

    const auto& [inter1, inter2] = selectRandomIntersections(intersections, minDistance);

    startPoint = &inter1;
    endPoint   = &inter2;

    openList.clear();
    closedList.clear();

    selectCurrentNode(startPoint->getId());
    currentNode->setDistanceTraveled(0);
    currentNode->setDistanceToTarget(startPoint->getNode().distance(endPoint->getNode()));
    openList.insert(*currentNode);

    solved = false;

    drawStartAndEndPoint(overlay.getCurrentTexture());
}

void Solver::selectCurrentNode(uint64_t id) {

    auto find = nodes.find(id);
    if (find == nodes.end())
        throw std::runtime_error("Node is not in list of all nodes. ID: " + std::to_string(id));

    currentNode = &(find->second);
    currentConnections = currentNode->getConnections();
    currentConnectionIterator = currentConnections.begin();
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

void Solver::doStepAndDraw(float speed, float roadPercentage) {

    if (isSolved())
        return;

    const uint32_t roadCount = static_cast<uint32_t>(static_cast<float>(openList.size()) * roadPercentage);

    const auto roads = doStep(speed, roadCount);
    std::vector<LineRenderer> lines;
    lines.reserve(roads.size());
    for (const Road& road : roads) {

        lines.emplace_back(road.getWay(), color);
    }

    LineBuffer buffer(lines, RenderConfig());

    buffer.draw(overlay.getCurrentTexture());

    drawStartAndEndPoint(overlay.getCurrentTexture());
}

std::vector<std::reference_wrapper<const Road>> Solver::doStep(float speed, uint32_t maxRoads) {

    if (maxRoads == 0)
        maxRoads = 1;

    std::vector<std::reference_wrapper<const Road>> roads;

    const float targetDistance = speed * 360.0f / 40'000'000.0f;

    const double startDistance = [&]() -> double{

        if (currentNode != nullptr)
            return currentNode->getDistanceTraveled();

        if (!openList.empty())
            return openList.begin()->get().getDistanceTraveled();

        return 0.0;
    }();

    while (roads.size() < maxRoads) {

        std::optional<Intersection::Connection> connection = checkNextRoad();

        if (connection.has_value()) {

            roads.emplace_back(connection->road);

            const PathNode& node = nodes.find(connection->intersection.getId())->second;

            if (node.getDistanceTraveled() - startDistance > targetDistance)
                break;

        } else if (solved == true) {

            break;
        }
    }

    return roads;
}

std::optional<Intersection::Connection> Solver::checkNextRoad() {

    if (currentNode == nullptr) {
        if (!selectNextNode()) {
            return {};
        }
    }

    const Intersection::Connection& currentConnection = *currentConnectionIterator;

    const uint64_t nextIntersectionId = currentConnection.intersection.getId();
    auto find = nodes.find(nextIntersectionId);
    if (find == nodes.end())
        throw std::runtime_error("Unable to find node in list of all nodes. Id: " + std::to_string(nextIntersectionId));
    PathNode& nextNode = find->second;

    if (closedList.contains(nextNode)) {
        advanceConnectionIterator();
        return {}; // Next node was allready checked
    }

    double newDistance = currentConnection.distance + currentNode->getDistanceTraveled();

    if (openList.contains(nextNode) && newDistance >= nextNode.getDistanceTraveled()) {
        advanceConnectionIterator();
        return {}; // There is allready a better connection to the next road
    }

    nextNode.setPredecessor(*currentNode, currentConnection.road);
    nextNode.setDistanceTraveled(newDistance);
    const double distanceToTarget = currentConnection.intersection.getNode().distance(endPoint->getNode());
    nextNode.setDistanceToTarget(distanceToTarget);

    // Elements in a set are constant. Therefor the element has to be removed
    // and reinserted for the key to be updated.
    auto [iter, success] = openList.insert(nextNode);
    if (!success) {
        openList.erase(iter);
        openList.insert(nextNode);
    }

    advanceConnectionIterator();

    return currentConnection;
}

void Solver::advanceConnectionIterator() {

    currentConnectionIterator++;
    if (currentConnectionIterator == currentConnections.end()) {
        currentNode = nullptr; // This node is done. Move to the next node.
    }
}

bool Solver::selectNextNode() {

    if (openList.empty())
        return false; // No more nodes in open list. Unable to find solution.

    // get the node with the lowest score from the open list and remove it
    auto iter = openList.begin();
    PathNode& currentNode = iter->get();

    if (currentNode.getIntersection() == *endPoint) {
        solved = true;
        return false;
    }
    
    openList.erase(iter);

    this->currentNode = &currentNode;
    currentConnections = currentNode.getConnections();
    currentConnectionIterator = currentConnections.begin();

    closedList.insert(currentNode);

    return true;
}

void Solver::drawPoint(const Intersection& intersection, sf::RenderTarget& target) {

    const uint8_t radiusInPixels = 3;

    const sf::Vector2f size = overlay.getSize();
    const sf::Vector2f res = {
        static_cast<float>(overlay.getResolution().x),
        static_cast<float>(overlay.getResolution().y)
    };

    const float radius = size.x / res.x * static_cast<float>(radiusInPixels);

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