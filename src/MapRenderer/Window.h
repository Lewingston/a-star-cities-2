#pragma once

#include "LineBuffer.h"
#include "MapRenderer.h"

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

            MapRenderer mapRenderer;

            void onEvent(const sf::Event& event);
            void onResize(uint32_t newWidth, uint32_t newHeight);
            void onMouseWheel(const sf::Event::MouseWheelScrolled& mouseWheelEvent);
            void onMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent);

            void draw();
            void drawCenter();

            sf::Vector2i mousePosition;

            sf::Vector2f mapCenter;

            float zoom = 1.0f;

    };
}