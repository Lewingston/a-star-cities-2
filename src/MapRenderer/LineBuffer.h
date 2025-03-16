#pragma once

#include "RenderConfig.h"

#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <map>
#include <cstdint>
#include <vector>

namespace asc2 {

    class Way;

    class LineBuffer {

        public:

            LineBuffer(const std::vector<std::reference_wrapper<const Way>>& ways, const RenderConfig& config);

            LineBuffer(const std::map<uint64_t, Way>& ways, const RenderConfig& config);

            void draw(sf::RenderTarget& target);

            [[nodiscard]] std::size_t getNumberOfLines() const noexcept {
                return vertexBuffer.getVertexCount() / 2;
            }

            [[nodiscard]] std::size_t getVertexCount() const noexcept {
                return vertexBuffer.getVertexCount();
            }

        private:

            RenderConfig config;

            sf::VertexBuffer vertexBuffer;

            std::vector<sf::Vertex> vertexArray;

            void initVertexBuffer(std::size_t vertexCount);

            [[nodiscard]] bool wayIsSuitableForRendering(const Way& way) const;

            void addWayToVertexArray(const Way& way,
                                     std::vector<sf::Vertex>& vertexArray) const;

            void updateVertexBuffer();

    };
}