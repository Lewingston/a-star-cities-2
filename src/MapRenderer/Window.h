#pragma once

#include "LineBuffer.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace asc2 {

    class Map;

    class Window {

        public:

            Window(uint32_t width, uint32_t height, const std::string& title);

            void show();
            void renderMap(const Map& map);

        private:

            sf::RenderWindow window;

            void onEvent(const sf::Event& event);
            void onResize(uint32_t newWidth, uint32_t newHeight);
            void draw();

            std::unique_ptr<LineBuffer> lineBuffer;

    };
}