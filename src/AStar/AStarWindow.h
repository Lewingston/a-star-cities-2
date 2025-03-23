#pragma once

#include "../MapRenderer/Window.h"

namespace asc2 {

    class LoadingWindow;

    class AStarWindow : public Window {

        public:

            AStarWindow(uint32_t width, uint32_t height, const std::string& title);

            void show() override;

            void setMapFile(const std::string& fileName) { mapFileName = fileName; }

        private:

            std::string mapFileName;

            void loadMap(const std::string& filePath);

            void loadThread(const std::string& filePath, LoadingWindow& window, std::unique_ptr<Map>& map);

    };
}