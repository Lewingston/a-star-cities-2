#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>

namespace asc2 {

    class AStarOverlay {

        public:

            AStarOverlay(const sf::Vector2u& size);

            void setResolution(sf::Vector2u resolution);
            void setSize(sf::Vector2f size);
            void setView(const sf::View& view);
            void setPosition(const sf::Vector2f& position);
            void setRotation(float rotation);
            
            void clear();

            sf::RenderTexture& getCurrentTexture() { return textures[index]; }

            sf::Sprite& getCurrentSprite() {
                return index == 0 ? sprite1 : sprite2;
            }

            void draw(sf::RenderTarget& target);

            [[nodiscard]] sf::Vector2f getSize() const { return size; };
            [[nodiscard]] sf::Vector2f getPosition() const { return sprite1.getPosition(); }

        private:

            void setResolution(sf::RenderTexture& texture, sf::Vector2u resolution) const;
            void updateSprites();

            std::array<sf::RenderTexture, 2> textures;
            sf::Sprite sprite1;
            sf::Sprite sprite2;

            sf::Vector2f size;

            uint8_t index = 0;
    };
}