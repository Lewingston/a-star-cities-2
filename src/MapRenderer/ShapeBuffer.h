#pragma once

#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

namespace sf {
    class RenderTarget;
}

namespace asc2 {

    class ShapeRenderer;

    class ShapeBuffer {

        public:

            ShapeBuffer(const std::vector<ShapeRenderer>& shapes, bool lineMode = false);

            void draw(sf::RenderTarget& target);

            [[nodiscard]] std::size_t getVertexCount() const noexcept;
            [[nodiscard]] std::size_t getEdgeCount() const noexcept;
            [[nodiscard]] std::size_t getPolygonCount() const noexcept;

        private:

            sf::VertexBuffer vertexBuffer;

            std::vector<sf::Vertex> vertexArray;

            void initVertexBuffer(std::size_t vertexCount);

            void addShapeToVertexArray(const ShapeRenderer& shape,
                                       std::vector<sf::Vertex>& vertexArray) const;

            void updateVertexBuffer();

            bool lineMode;
    };
}