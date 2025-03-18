#pragma once

#include "RenderBufferInterface.h"

#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

namespace asc2 {

    class ShapeRenderer;
    class NodeRenderer;

    class ShapeBuffer : public RenderBufferInterface {

        public:

            ShapeBuffer(const std::vector<ShapeRenderer>& shapes, bool lineMode = false);

            ShapeBuffer(const std::vector<NodeRenderer>& nodes);

            void draw(sf::RenderTarget& target) override;

            [[nodiscard]] std::size_t getVertexCount() const noexcept override;
            [[nodiscard]] std::size_t getLineCount() const noexcept override;
            [[nodiscard]] std::size_t getPolygonCount() const noexcept override;

        private:

            sf::VertexBuffer vertexBuffer;

            std::vector<sf::Vertex> vertexArray;

            void initVertexBuffer(std::size_t vertexCount);

            void addShapeToVertexArray(const ShapeRenderer& shape,
                                       std::vector<sf::Vertex>& vertexArray) const;

            void updateVertexBuffer();

            bool lineMode = false;
    };
}