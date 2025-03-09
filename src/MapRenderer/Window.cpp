
#include "Window.h"
#include "../Map/Map.h"

using namespace asc2;

Window::Window(uint32_t width, uint32_t height, const std::string& title) :
    window(sf::VideoMode({width, height}), title) {

}

void Window::show() {

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {

            if (event.has_value())
                onEvent(event.value());
        }

        draw();
    }
}

void Window::onEvent(const sf::Event& event) {

    if (event.is<sf::Event::Closed>()) {

        window.close();

    } else if (const auto* resized = event.getIf<sf::Event::Resized>()) {

        onResize(resized->size.x, resized->size.y);
    }
}

void Window::draw() {

    window.clear(sf::Color::White);

    if (lineBuffer)
        lineBuffer->draw(window);

    window.display();
}

void Window::onResize(uint32_t newWidth, uint32_t newHeight) {

    auto view = window.getView();
    view.setSize(sf::Vector2f(static_cast<float>(newWidth), static_cast<float>(newHeight)));
    window.setView(view);
}

void Window::renderMap(const Map& map) {

    lineBuffer = std::make_unique<LineBuffer>(map.getAllWays());
}