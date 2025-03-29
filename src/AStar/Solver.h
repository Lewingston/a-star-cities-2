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

            void setMap(std::shared_ptr<Map> map) { this->map = map; }

            void selectStartAndEndNode();

            void drawStartAndEndPoint(sf::RenderTarget& target);

        private:

            std::vector<std::reference_wrapper<const Intersection>> getIntersectionsInArea(sf::Vector2f center, sf::Vector2f size, float rotation) const;
            std::vector<std::reference_wrapper<const Intersection>> getIntersectionsInArea(const std::array<sf::Vector2f, 4>& area) const;

            const Intersection& selectRandomIntersection(const std::vector<std::reference_wrapper<const Intersection>>& intersections);
            std::pair<const Intersection&, const Intersection&> selectRandomIntersections(const std::vector<std::reference_wrapper<const Intersection>>& intersection, float distance);

            void drawPoint(const Intersection& intersection, sf::RenderTarget& target);

            AStarOverlay& overlay;
            std::shared_ptr<Map> map;

            std::mt19937 randomGenerator;

            const Intersection* startPoint = nullptr;
            const Intersection* endPoint = nullptr;

            sf::Color color = sf::Color::White;
    };
}