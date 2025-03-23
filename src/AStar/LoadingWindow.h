#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include <mutex>

namespace asc2 {

    class LoadingWindow {

        public:

            LoadingWindow(sf::RenderWindow& window) :
                window(window) {}

            void show();

            void setProgress(uint32_t currentProgress, uint32_t maxProgress);

            void addStatusMessage(const std::string& message);

            void removeLastStatusMessage();

            void finishedLoading();

        private:

            sf::RenderWindow& window;

            sf::Font font;

            uint32_t currentProgress = 0;
            uint32_t maxProgress = 1;

            std::string statusText;

            std::mutex progressMutex;
            std::mutex statusTextMutex;

            bool loadingInProgress = true;

            void loadFont();

            void onEvent(const sf::Event& event);

            void draw();

    };
}