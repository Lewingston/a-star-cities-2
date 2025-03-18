#pragma once

#include <cstdlib>

namespace sf {
    class RenderTarget;
}

namespace asc2 {

    class RenderBufferInterface {

        public:

            virtual ~RenderBufferInterface() = default;

            [[nodiscard]] virtual std::size_t getVertexCount() const noexcept = 0;

            [[nodiscard]] virtual std::size_t getLineCount() const noexcept = 0;

            [[nodiscard]] virtual std::size_t getPolygonCount() const noexcept = 0;

            virtual void draw(sf::RenderTarget& target) = 0;

    };
}