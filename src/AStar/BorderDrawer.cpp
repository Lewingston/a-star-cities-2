
#include "BorderDrawer.h"

#include <SFML/System/Angle.hpp>

using namespace asc2;

void BorderDrawer::update(sf::Vector2f center, sf::Vector2f size, float angel) {

    this->rotation = angel;
    this->center   = center;
    this->size     = size;

    points[0] = sf::Vector2f(size.x / 2.0f, size.y / 2.0f);
    points[1] = sf::Vector2f(size.x / 2.0f, -size.y / 2.0f);
    points[2] = sf::Vector2f(-size.x / 2.0f, -size.y / 2.0f);
    points[3] = sf::Vector2f(-size.x / 2.0f, size.y / 2.0f);

    const auto transform = [&](sf::Vector2f point) -> sf::Vector2f {
        return point.rotatedBy(sf::degrees(angel)) + center;
    };

    for (auto& point : points)
        point = transform(point);
}

void BorderDrawer::draw(sf::RenderTarget& target) {

    const std::array<sf::Vertex, 5> border {
        sf::Vertex{points[0], color},
        sf::Vertex{points[1], color},
        sf::Vertex{points[2], color},
        sf::Vertex{points[3], color},
        sf::Vertex{points[0], color},
    };

    target.draw(border.data(), border.size(), sf::PrimitiveType::LineStrip);
}