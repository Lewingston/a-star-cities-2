#pragma once

#include "AStarOverlay.h"
#include "BorderDrawer.h"
#include "Solver.h"
#include "../MapRenderer/Window.h" 
#include "../MapRenderer/ShapeBuffer.h"

namespace asc2 {

    class LoadingWindow;

    class AStarWindow : public Window {

        public:

            AStarWindow(uint32_t width, uint32_t height, const std::string& title, bool interactive);

            void show() override { Window::show(); }
            void show(MapLoader& mpaLoader, const RenderConfig& config) override;

        private:

            void draw() override;

            void drawImGui() override;

            void setRotation(float angel);

            void onResize(uint32_t newWidth, uint32_t newHeight) override;

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
            void onKeyPressed(const sf::Event::KeyPressed& keyPressed) override;

            [[nodiscard]] std::vector<std::reference_wrapper<const Intersection>> getIntersectionsInArea(const std::array<sf::Vector2f, 4>& area) const;

            std::shared_ptr<Map> map;

            AStarOverlay overlay;
            Solver solver;

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

            float animationSpeed = 150.0f;
            float roadPercentage = 100.0f / 10.0f;
            float speedMultiplier = 1.0f;
            int fadeSpeed = 6;

            std::chrono::steady_clock::time_point timer = std::chrono::steady_clock::now();

            bool interactive;
    };
}