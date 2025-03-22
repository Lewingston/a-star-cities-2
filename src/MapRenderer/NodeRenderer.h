#pragma once

#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>

namespace asc2 {

    class NodeRenderer {

        public:

            NodeRenderer(float posX, float posY, sf::Color color);

            static void setGeometry(float size, uint8_t vertexCount);

            [[nodiscard]] static uint8_t getVertexCount() { return vertexCount; }

            [[nodiscard]] const std::vector<std::pair<float, float>>& getVertices() const;

            [[nodiscard]] std::pair<float, float> getPosition() const noexcept { return { posX, posY }; }

            [[nodiscard]] sf::Color getColor() const noexcept { return color; }

        private:

            float posX;
            float posY;

            sf::Color color;

            static float size;
            static uint8_t vertexCount;

            static std::vector<std::pair<float, float>> vertices;

            [[nodiscard]] static std::vector<std::pair<float, float>> createVertices(float size, uint8_t vertexCount);
    };
}