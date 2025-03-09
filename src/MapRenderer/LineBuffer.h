#pragma once

#include <SFML/Graphics/VertexBuffer.hpp>

#include <map>
#include <cstdint>
#include <vector>

namespace asc2 {

    class Way;

    class LineBuffer {

        public:

            LineBuffer(const std::map<uint64_t, Way>& ways);

            void draw(sf::RenderTarget& target);

        private:

            sf::VertexBuffer vertexBuffer;

            void addWayToVertexArray(const Way& way,
                                     std::vector<sf::Vertex>& vertexArray) const;
    };
}