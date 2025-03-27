
#include "AStarWindow.h"
#include "../MapLoader/LoadingWindow.h"
#include "../MapLoader/MapLoader.h"
#include "../MapParser/MapParser.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/System/Angle.hpp>

#include <iostream>
#include <thread>

using namespace asc2;

AStarWindow::AStarWindow(uint32_t width, uint32_t height, const std::string& title) :
    Window(width, height, title),
    overlay(sf::Vector2u(width, height)) {

    showMapCenter = false;
    showMapBorder = false;
}

void AStarWindow::show(MapLoader& mapLoader, const RenderConfig& config) {

    LoadingWindow loadingWindow(window);

    map = std::make_shared<Map>();

    mapLoader.startLoader({&loadingWindow, map});

    loadingWindow.show();

    mapLoader.join();

    renderMap(*map, config);
    Window::show();
}

void AStarWindow::renderMap(const Map& map, const RenderConfig& config) {

    Window::renderMap(map, config);

    const sf::Vector2f position = {
        static_cast<float>(map.getCenter().first),
        static_cast<float>(-map.getCenter().second)
    };

    const sf::Vector2f size = {
        static_cast<float>(map.getSize().first),
        static_cast<float>(map.getSize().second)
    };

    applyNewView(position, size, 0.0f);
}

void AStarWindow::applyNewView(sf::Vector2f center, sf::Vector2f size, float rotation) {
    
    const float windowWidth  = static_cast<float>(window.getSize().x);
    const float windowHeight = static_cast<float>(window.getSize().y);

    if (size.x / size.y > windowWidth / windowHeight) {
        zoom = size.x / windowWidth;
    } else {
        zoom = size.y / windowHeight;
    }

    auto view = window.getView();
    view.setSize(sf::Vector2f(windowWidth, windowHeight));
    view.setCenter(center);
    view.setRotation(sf::degrees(rotation));
    view.zoom(zoom);
    window.setView(view);

    const float windowRatio = std::abs(windowWidth / windowHeight);
    const float mapRatio = std::abs(size.x / size.y);
    const bool moreWide = mapRatio > windowRatio;

    const sf::Vector2u resolution = {
        moreWide ? window.getSize().x : static_cast<uint32_t>(static_cast<float>(window.getSize().y) * mapRatio / windowRatio),
        moreWide ? static_cast<uint32_t>(static_cast<float>(window.getSize().x) * windowRatio / mapRatio) : window.getSize().y 
    };

    overlay.setResolution(resolution);
    overlay.setPosition(center);
    overlay.setSize(size);
    overlay.setRotation(rotation);
    overlay.setView(window.getView());

    currentOverlayBorders.update(center, size, rotation);
}

void AStarWindow::draw() {

    Window::draw();

    if (drawOverlayBoundsActive)
        currentOverlayBorders.draw(window);

    if (redrawMode)
        newOverlayBorders.draw(window);

    overlay.draw(window);
}

void AStarWindow::drawImGui() {

    //ImGui::ShowDemoWindow();

    drawInfoOverlay();

    displayUi();
}

void AStarWindow::displayUi() {

    ImGui::Begin("AStar");

    if (ImGui::CollapsingHeader("Info")) {

        ImGui::Text("Main roads:         %llu", map->getMainRoadCount());
        ImGui::Text("Main intersections: %llu", map->getMainIntersections().size());
        ImGui::Text("All roads:          %llu", map->getAllRoads().size());
        ImGui::Text("All intersections:  %llu", map->getAllIntersections().size());
    }

    if (ImGui::CollapsingHeader("Controls")) {

        ImGui::Checkbox("Show map center", &showMapCenter);
        ImGui::Checkbox("Show map border", &showMapBorder);
        ImGui::Checkbox("Show overlay border", &drawOverlayBoundsActive);

        if (ImGui::Button("Reset View Port")) {
            resetView();
        }

        ImGui::SliderFloat("Map rotation", &rotationInput, -180.0f, 180.0f);

        ImGui::InputScalar("Map rotation input:", ImGuiDataType_Float, &rotationInput);

        if (mapRotation != rotationInput) {
            setRotation(rotationInput);
        }

        if (!redrawMode) {
            if (ImGui::Button("Redraw overlay")) {
                redrawMode = true;
            }
        } else {
            if (ImGui::Button("Ok")) {
                applyNewView(newOverlayBorders.getCenter(), newOverlayBorders.getSize(), newOverlayBorders.getRotation());
                redrawMode = false;
            }
        }
    }

    ImGui::End();
}

void AStarWindow::onMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent) {

    if (!leftMouseButtonPressed || !redrawMode) {
        Window::onMouseMoved(mouseMovedEvent);
        return;
    }

    if (redrawMode) {

        endPoint = window.mapPixelToCoords(mouseMovedEvent.position, window.getView());

        sf::Vector2f size = endPoint - startPoint;
        const sf::Vector2f center = startPoint + (size / 2.0f);

        size = size.rotatedBy(-window.getView().getRotation());

        newOverlayBorders.update(center, size, window.getView().getRotation().asDegrees());
    }
}

void AStarWindow::onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButton) {

    Window::onMouseButtonPressed(mouseButton);

    if (mouseButton.button == sf::Mouse::Button::Left) {
        startPoint = window.mapPixelToCoords(mouseButton.position, window.getView());
        endPoint = startPoint;
    }
}

void AStarWindow::onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButton) {

    Window::onMouseButtonReleased(mouseButton);
}

void AStarWindow::setRotation(float angel) {

    mapRotation = angel;
    rotationInput = angel;

    auto view = window.getView();
    view.setRotation(sf::degrees(angel));
    window.setView(view);
}

void AStarWindow::resetView() {

    Window::resetView();

    mapRotation = 0.0f;
    rotationInput = 0.0f;
}