
#include "LoadingWindow.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

extern unsigned char robotoRegular[];
extern unsigned int robotoRegular_len;

using namespace asc2;

void LoadingWindow::loadFont() {

    if (!font.openFromMemory(&robotoRegular, robotoRegular_len)) {
        throw std::runtime_error("Failed to load font!");
    }
}

void LoadingWindow::show() {

    loadFont();

    while (window.isOpen() && loadingInProgress) {

        while (const std::optional event = window.pollEvent()) {

            if (event.has_value()) {
                onEvent(event.value());
            }
        }

        draw();

        window.display();
    }
}

void LoadingWindow::setProgress(uint32_t currentProgress, uint32_t maxProgress) {

    const std::lock_guard<std::mutex> lock(progressMutex);

    if (maxProgress == 0)
        maxProgress = 1;

    if (currentProgress > maxProgress)
        currentProgress = maxProgress;

    this->currentProgress = currentProgress;
    this->maxProgress     = maxProgress;
}

void LoadingWindow::addStatusMessage(const std::string& message) {

    const std::lock_guard<std::mutex> lock(statusTextMutex);

    if (!statusText.empty())
        statusText += '\n';

    statusText += message;
}

void LoadingWindow::removeLastStatusMessage() {

    const std::lock_guard<std::mutex> lock(statusTextMutex);

    std::size_t index = statusText.find_last_of('\n');
    if (index != std::string::npos) {
        statusText.erase(index);
    } else {
        statusText.clear();
    }
}

void LoadingWindow::finishedLoading() {

    loadingInProgress = false;
}

void LoadingWindow::onEvent(const sf::Event& event) {

    if (event.is<sf::Event::Closed>()) {
        window.close();
    }
}

void LoadingWindow::draw() {

    const sf::Vector2f windowSize = window.getView().getSize();

    static constexpr sf::Vector2f loadingBarSize(1000.0f, 60.0f);

    window.clear(sf::Color::Black);

    { // Draw loading text
    sf::Text text(font, "LOADING", 100);
    text.setFillColor(sf::Color::White);

    const sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition((windowSize - textBounds.size) / 2.0f);
    text.move(sf::Vector2f(0.0f, -330.0f));

    window.draw(text);
    }

    const sf::Vector2f loadingBarPos = (windowSize - loadingBarSize) / 2.0f - sf::Vector2f(0.0f, 120.0f);

    { // Draw progress
    const std::lock_guard<std::mutex> lock(progressMutex);

    const float progress = 1 - static_cast<float>(currentProgress) / static_cast<float>(maxProgress);
    const sf::Vector2f progressBarSize = loadingBarSize - sf::Vector2f(loadingBarSize.x * progress, 0.0f);

    sf::RectangleShape progressBar(progressBarSize);
    progressBar.setPosition(loadingBarPos);
    progressBar.setFillColor(sf::Color::White);

    window.draw(progressBar);
    }

    { // Draw loading bar
    sf::RectangleShape loadingBar(loadingBarSize);
    loadingBar.setPosition(loadingBarPos);
    loadingBar.setFillColor(sf::Color::Transparent);
    loadingBar.setOutlineColor(sf::Color::White);
    loadingBar.setOutlineThickness(4.0f);

    window.draw(loadingBar);
    }

    { // Draw status text
    const std::lock_guard<std::mutex> lock(statusTextMutex);

    sf::Text text(font, statusText, 24);
    text.setFillColor(sf::Color::White);

    const sf::FloatRect textBounds = text.getLocalBounds();
    //text.setPosition((windowSize - textBounds.size) / 2.0f);
    //text.move(sf::Vector2f(0.0f, 30.0f));

    const float posX = (windowSize.x - textBounds.size.x) / 2.0f;
    const float posY = 400.0f;

    text.setPosition(sf::Vector2f(posX, posY));
    
    window.draw(text);
    }
}