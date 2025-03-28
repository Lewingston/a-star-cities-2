#pragma once

#include <SFML/System/Vector2.hpp>

#include <array>

namespace asc2 {

    class Geometry {

        public:

            struct Rectangle {

                sf::Vector2f pointA() const { return points[0]; }
                sf::Vector2f pointB() const { return points[1]; }
                sf::Vector2f pointC() const { return points[2]; }
                sf::Vector2f pointD() const { return points[3]; }

                std::array<sf::Vector2f, 4> points;
            };

            [[nodiscard]] static bool pointInRectangle(sf::Vector2f point, const Rectangle& rectangle);
    };
}