#pragma once

#include "RenderBufferInterface.h"
#include "RenderConfig.h"

#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <map>
#include <cstdint>
#include <vector>

namespace asc2 {

    class Way;
    class LineRenderer;

    class LineBuffer : public RenderBufferInterface {

        public:

            LineBuffer(const std::vector<std::reference_wrapper<const Way>>& ways, const RenderConfig& config);

            LineBuffer(const std::map<uint64_t, Way>& ways, const RenderConfig& config);

            LineBuffer(const std::vector<LineRenderer>& lines, const RenderConfig& config);

            void draw(sf::RenderTarget& target) override;

            [[nodiscard]] std::size_t getVertexCount() const noexcept override {
                return vertexBuffer.getVertexCount();
            }

            [[nodiscard]] std::size_t getLineCount() const noexcept override {
                return vertexBuffer.getVertexCount() / 2;
            }

            [[nodiscard]] std::size_t getPolygonCount() const noexcept override {
                return 0;
            }

        private:

            RenderConfig config;

            sf::VertexBuffer vertexBuffer;

            std::vector<sf::Vertex> vertexArray;

            void initVertexBuffer(std::size_t vertexCount);

            [[nodiscard]] bool wayIsSuitableForRendering(const Way& way) const;

            void addWayToVertexArray(const Way& way,
                                     std::vector<sf::Vertex>& vertexArray,
                                     sf::Color color) const;

            void updateVertexBuffer();

    };
}