#pragma once

#include "AStarOverlay.h"
#include "BorderDrawer.h"
#include "../MapRenderer/Window.h" 

namespace asc2 {

    class LoadingWindow;

    class AStarWindow : public Window {

        public:

            AStarWindow(uint32_t width, uint32_t height, const std::string& title);

            void show() override { Window::show(); }
            void show(MapLoader& mpaLoader, const RenderConfig& config) override;

        private:

            void draw() override;

            void drawImGui() override;

            void setRotation(float angel);

            void resetView() override;
            void setView(const BorderDrawer& borders);
            void setView(sf::Vector2f center, sf::Vector2f size, float rotation);

            void renderMap(const Map& map, const RenderConfig& config) override;

            void displayUi();
            void infoOverlay();

            void initOverlay(sf::RenderTexture& texture);
            void applyNewView(sf::Vector2f center, sf::Vector2f size, float rotation);
            void resetOverlay();

            void onMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent) override;
            void onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButton) override;
            void onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButton) override;

            std::shared_ptr<Map> map;

            AStarOverlay overlay;

            float mapRotation = 0.0f;
            float rotationInput = 0.0f;

            bool drawOverlayBoundsActive = false;

            bool redrawMode = false;
            bool currentlyDrawing = false;

            bool showInfoOverlay = true;

            BorderDrawer currentOverlayBorders;
            BorderDrawer newOverlayBorders;

            sf::Vector2f startPoint;
            sf::Vector2f endPoint;

    };
}