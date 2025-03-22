#pragma once

#include <SFML/Graphics/Color.hpp>

namespace asc2 {

    class Way;

    class LineRenderer {

        public:

            LineRenderer(const Way& way, sf::Color color) :
                way(way), color(color) { }

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

            [[nodiscard]] sf::Color getColor() const noexcept { return color; }

        private:

            const Way& way;
            
            sf::Color color;

    };
}