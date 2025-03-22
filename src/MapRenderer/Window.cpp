
#include "Window.h"
#include "../Map/Map.h"

#include <iostream>

using namespace asc2;

Window::Window(uint32_t width, uint32_t height, const std::string& title) :
    window(sf::VideoMode({width, height}), title) {

    window.setFramerateLimit(60u);
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

    } else if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {

        onMouseWheel(*mouseWheel);

    } else if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {

        onMouseMoved(*mouseMove);
    }
}

void Window::draw() {

    window.clear(config.backgroundColor);

    mapRenderer.draw(window);

    drawCenter();

    window.display();
}

void Window::drawCenter() {

    const float size = 180.0f / 40'000'000 * 15.0f;
    const sf::Color color = sf::Color::Red;

    std::array center = {
        sf::Vertex{sf::Vector2f(size, size) + mapCenter, color},
        sf::Vertex{sf::Vector2f(-size, -size) + mapCenter, color},
        sf::Vertex(sf::Vector2f(-size, size) + mapCenter, color),
        sf::Vertex(sf::Vector2f(size, -size) + mapCenter, color)
    };

    window.draw(center.data(), center.size(), sf::PrimitiveType::Lines);
}

void Window::onResize(uint32_t newWidth, uint32_t newHeight) {

    auto view = window.getView();
    view.setSize(sf::Vector2f(static_cast<float>(newWidth), static_cast<float>(newHeight)));
    view.zoom(zoom);
    window.setView(view);
}

void Window::onMouseWheel(const sf::Event::MouseWheelScrolled& mouseWheelEvent) {

    auto view = window.getView();

    const float zoomSpeed = 0.2f;
    float zoom = mouseWheelEvent.delta * -(1.0f + zoomSpeed);

    if (zoom < 0)
        zoom = -1.0f / zoom;

    const sf::Vector2f oldMousePos = window.mapPixelToCoords(mouseWheelEvent.position);
    
    this->zoom *= zoom;

    view.zoom(zoom);
    window.setView(view);

    // make sure mouse stays over the same position while zooming
    const sf::Vector2f newMousePos = window.mapPixelToCoords(mouseWheelEvent.position);
    view.move(oldMousePos - newMousePos);
    window.setView(view);
}

void Window::onMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

        auto view = window.getView();

        const auto oldPos = window.mapPixelToCoords(mousePosition);
        const auto newPos = window.mapPixelToCoords(mouseMovedEvent.position);

        view.move(oldPos - newPos);
        window.setView(view);
    }

    mousePosition = mouseMovedEvent.position;
}

void Window::renderMap(const Map& map,
                       const RenderConfig& config) {

    this->config = config;

    mapRenderer.init(map, config);

    mapCenter = {static_cast<float>(map.getCenter().first), -static_cast<float>(map.getCenter().second)};

    zoom = 180.0f / 40'000'000.0f;

    auto view = window.getView();
    view.setCenter(mapCenter);
    view.zoom(zoom);
    window.setView(view);
}