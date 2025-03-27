#pragma once

#include "LineBuffer.h"
#include "MapRenderer.h"
#include "../Map/Map.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace asc2 {

    class Map;
    class MapLoader;

    struct RenderConfig;

    class Window {

        public:

            Window(uint32_t width, uint32_t height, const std::string& title);

            virtual ~Window() = default;

            virtual void show();

            virtual void show(MapLoader& mapLoader, const RenderConfig& config);

            virtual void renderMap(const Map& map,
                                   const RenderConfig& config);

        protected:

            sf::RenderWindow window;

            MapRenderer mapRenderer;

            RenderConfig config;

            void onEvent(const sf::Event& event);
            void onResize(uint32_t newWidth, uint32_t newHeight);
            void onMouseWheel(const sf::Event::MouseWheelScrolled& mouseWheelEvent);
            virtual void onMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent);
            virtual void onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButton);
            virtual void onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButton);

            virtual void resetView();

            virtual void draw();
            void drawCenter();

            virtual void drawImGui();
            virtual void drawInfoOverlay();

            void createMapBorder(float posX, float posY, float width, float height);

            sf::Vector2i mousePosition;

            Map::Dimensions mapDimensions;

            std::array<sf::Vertex, 5> mapBorder;

            bool leftMouseButtonPressed = false;
            bool rightMouseButtonPressed = false;

            float zoom = 1.0f;

            bool showMapBorder = true;
            bool showMapCenter = true;

    };
}