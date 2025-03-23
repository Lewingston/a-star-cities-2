
#include "Window.h"
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../MapLoader/LoadingWindow.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

using namespace asc2;

Window::Window(uint32_t width, uint32_t height, const std::string& title) :
    window(sf::VideoMode({width, height}), title) {

    window.setFramerateLimit(60u);
}

void Window::show(MapLoader& mapLoader, const RenderConfig& config) {

    LoadingWindow loadingWindow(getWindow());

    std::shared_ptr<Map> map = std::make_shared<Map>();

    mapLoader.startLoader({&loadingWindow, map});

    loadingWindow.show();

    mapLoader.join();

    if (map != nullptr) {
        renderMap(*map, config);
        show();
    }
}

void Window::show() {

    /*if (!ImGui::SFML::Init(window)) {
        throw std::runtime_error("Failed to initialize ImGui.");
    }*/

    sf::Clock deltaClock;

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {

            if (event.has_value()) {
                //ImGui::SFML::ProcessEvent(window, event.value());

                //if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
                    onEvent(event.value());
            }
        }

        //ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowDemoWindow();

        draw();

        //ImGui::SFML::Render(window);

        window.display();
    }

    //ImGui::SFML::Shutdown();
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

    } else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {

        if (keyPressed->code == sf::Keyboard::Key::C) {
            resetView();
        }
    }
}

void Window::draw() {

    window.clear(config.backgroundColor);

    mapRenderer.draw(window);

    drawCenter();

    window.draw(mapBorder.data(), mapBorder.size(), sf::PrimitiveType::LineStrip);
}

void Window::drawCenter() {

    const float size = 180.0f / 40'000'000 * 15.0f;
    const sf::Color color = sf::Color::Red;

    sf::Vector2f mapCenter{
        static_cast<float>(mapDimensions.getCenter().first),
        static_cast<float>(-mapDimensions.getCenter().second)
    };

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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {

        auto view = window.getView();

        const auto oldPos = window.mapPixelToCoords(mousePosition);
        const auto newPos = window.mapPixelToCoords(mouseMovedEvent.position);

        view.move(oldPos - newPos);
        window.setView(view);
    }

    mousePosition = mouseMovedEvent.position;
}

void Window::resetView() {

    float x = static_cast<float>(mapDimensions.getCenter().first);
    float y = static_cast<float>(-mapDimensions.getCenter().second);

    const double mapWidth  = mapDimensions.maxLon - mapDimensions.minLon;
    const double mapHeight = mapDimensions.maxLat - mapDimensions.minLat;

    const double windowWidth  = static_cast<double>(window.getSize().x);
    const double windowHeight = static_cast<double>(window.getSize().y);

    if (mapWidth / mapHeight > windowWidth / windowHeight) {
        zoom = static_cast<float>(mapWidth / windowWidth);
    } else {
        zoom = static_cast<float>(mapHeight / windowHeight);
    }

    auto view = window.getView();
    view.setSize(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight))); // reset size, to reset zoom level
    view.setCenter(sf::Vector2f(x, y));
    view.zoom(zoom);
    window.setView(view);
}

void Window::renderMap(const Map& map,
                       const RenderConfig& config) {

    this->config = config;

    mapRenderer.init(map, config);

    mapDimensions = map.getDimensions();

    const double mapWidth  = map.getDimensions().maxLon - map.getDimensions().minLon;
    const double mapHeight = map.getDimensions().maxLat - map.getDimensions().minLat;

    zoom = 180.0f / 40'000'000.0f;

    createMapBorder(static_cast<float>(map.getDimensions().minLon), 
                    static_cast<float>(-map.getDimensions().minLat),
                    static_cast<float>(mapWidth),
                    static_cast<float>(mapHeight));

    resetView();
}

void Window::createMapBorder(float posX, float posY, float width, float height) {

    sf::Color color = sf::Color::Red;

    mapBorder = {
        sf::Vertex{sf::Vector2f(posX,         posY),          color},
        sf::Vertex{sf::Vector2f(posX + width, posY),          color},
        sf::Vertex(sf::Vector2f(posX + width, posY - height), color),
        sf::Vertex(sf::Vector2f(posX,         posY - height), color),
        sf::Vertex{sf::Vector2f(posX,         posY),          color}
    };
}