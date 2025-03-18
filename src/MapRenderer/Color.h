#pragma once

#include <SFML/Graphics/Color.hpp>

#include <random>

namespace asc2 {

    class Color {

        public:

            [[nodiscard]] static sf::Color getRandomColor() {

                const uint8_t r = static_cast<uint8_t>(std::rand() % 256);
                const uint8_t g = static_cast<uint8_t>(std::rand() % 256);
                const uint8_t b = static_cast<uint8_t>(std::rand() % 256);
                return sf::Color(r, g, b);
            }
    };
}