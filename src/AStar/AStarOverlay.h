#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>

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
            
            void setFadeSpeed(int speed) { fadeSpeed = speed; }

            void clear();

            sf::RenderTexture& getCurrentTexture() { return textures[index]; }

            sf::Sprite& getCurrentSprite() {
                return index == 0 ? sprite1 : sprite2;
            }

            void draw(sf::RenderTarget& target);

            void flip();

            [[nodiscard]] sf::Vector2f getSize() const { return size; };
            [[nodiscard]] sf::Vector2f getPosition() const { return sprite1.getPosition(); }
            [[nodiscard]] sf::Vector2u getResolution() const { return textures[0].getSize(); }
            [[nodiscard]] float getRotation() const { return sprite1.getRotation().asDegrees(); }

        private:

            void setResolution(sf::RenderTexture& texture, sf::Vector2u resolution) const;
            void updateSprites();

            void loadShaders();

            std::array<sf::RenderTexture, 2> textures;
            sf::Sprite sprite1;
            sf::Sprite sprite2;

            sf::Vector2f size;

            uint8_t index = 0;

            bool shadersAvailable = false;
            sf::Shader fadeShader;

            int fadeSpeed = 15;
            int currentFadeFrame = 0;
    };
}