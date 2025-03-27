#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace asc2 {

    class BorderDrawer {

        public:

            void update(sf::Vector2f center, sf::Vector2f size, float rotation);

            void draw(sf::RenderTarget& target);

            [[nodiscard]] float getRotation() const noexcept { return rotation; }
            [[nodiscard]] sf::Vector2f getCenter() const noexcept { return center; }
            [[nodiscard]] sf::Vector2f getSize() const noexcept { return size; }

        private:

            sf::Color color = sf::Color::White;

            std::array<sf::Vector2f, 4> points;

            float rotation = 0.0f;
            sf::Vector2f center;
            sf::Vector2f size;
    };
}