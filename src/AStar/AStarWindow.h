#pragma once

#include "../MapRenderer/Window.h"

namespace asc2 {

    class LoadingWindow;

    class AStarWindow : public Window {

        public:

            AStarWindow(uint32_t width, uint32_t height, const std::string& title);

            void show() override { Window::show(); }
            void show(MapLoader& mpaLoader, const RenderConfig& config) override;

        private:

            void drawImGui() override;

            void setRotation(float angel);

            void resetView() override;

            void displayUi();
            void infoOverlay();

            std::shared_ptr<Map> map;

            float mapRotation = 0.0f;
            float rotationInput = 0.0f;

    };
}