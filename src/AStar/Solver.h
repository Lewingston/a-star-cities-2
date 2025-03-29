#pragma once

#include "../Map/Map.h"
#include "../Map/Intersection.h"
#include "AStarOverlay.h"

#include <SFML/Graphics/Color.hpp>

#include <utility>
#include <random>

namespace sf {
    class RenderTarget;
}

namespace asc2 {

    class Solver {

        public:

            Solver(AStarOverlay& overlay, std::shared_ptr<Map> map);

            void setMap(std::shared_ptr<Map> map);

            void selectStartAndEndNode();

            void drawStartAndEndPoint(sf::RenderTarget& target);

            std::vector<std::reference_wrapper<const Road>> doStep(float speed, uint32_t maxRoads);
            void doStepAndDraw(float speed, float roadPercentage);

            void reset();

            void restart();

            [[nodiscard]] bool isSolved() const noexcept { return solved; }

        private:

            class PathNode {

                public:

                    PathNode(const Intersection& intersection) :
                        intersection(intersection) {}

                    bool operator<(const PathNode& node) const {
                        if (node.getIntersection() == getIntersection()) {
                            return false;
                        } else if (node.getScore() == getScore()) {
                            return node.getIntersection() < getIntersection();
                        } else {
                            return node.getScore() > getScore();
                        }
                    }

                    [[nodiscard]] const Intersection& getIntersection() const noexcept { return intersection; }

                    [[nodiscard]] double getDistanceToTarget() const noexcept { return distanceToTarget; }
                    [[nodiscard]] double getDistanceTraveled() const noexcept { return distanceTraveled; }

                    [[nodiscard]] double getScore() const noexcept { return getDistanceToTarget() + getDistanceTraveled(); }

                    void setDistanceToTarget(double distance) { distanceToTarget = distance; }
                    void setDistanceTraveled(double distance) { distanceTraveled = distance; }

                    void setPredecessor(const PathNode& prev, const Road& road) {
                        prevNode   = &prev;
                        roadToPrev = &road;
                    }

                    [[nodiscard]] std::vector<Intersection::Connection> getConnections() const { return intersection.getConnections(true); }

                private:

                    const Intersection& intersection;
                    const PathNode*     prevNode   = nullptr;
                    const Road*         roadToPrev = nullptr;

                    double distanceTraveled = std::numeric_limits<double>::max();
                    double distanceToTarget = std::numeric_limits<double>::max();
            };

            void initNodes();

            std::vector<std::reference_wrapper<const Intersection>> getIntersectionsInArea(sf::Vector2f center, sf::Vector2f size, float rotation) const;
            std::vector<std::reference_wrapper<const Intersection>> getIntersectionsInArea(const std::array<sf::Vector2f, 4>& area) const;

            const Intersection& selectRandomIntersection(const std::vector<std::reference_wrapper<const Intersection>>& intersections);
            std::pair<const Intersection&, const Intersection&> selectRandomIntersections(const std::vector<std::reference_wrapper<const Intersection>>& intersection, float distance);

            void drawPoint(const Intersection& intersection, sf::RenderTarget& target);

            void selectCurrentNode(uint64_t id);

            std::optional<Intersection::Connection> checkNextRoad();
            bool selectNextNode();
            void advanceConnectionIterator();

            AStarOverlay& overlay;
            std::shared_ptr<Map> map;

            std::mt19937 randomGenerator;

            const Intersection* startPoint = nullptr;
            const Intersection* endPoint = nullptr;

            sf::Color color = sf::Color::White;

            std::map<uint64_t, PathNode> nodes;

            using PriorityQueue = std::set<std::reference_wrapper<PathNode>, std::less<PathNode>>;

            PriorityQueue openList;
            std::set<std::reference_wrapper<PathNode>, std::less<PathNode>> closedList;

            PathNode* currentNode = nullptr;
            std::vector<Intersection::Connection> currentConnections;
            std::vector<Intersection::Connection>::iterator currentConnectionIterator;

            bool solved = false;
    };
}