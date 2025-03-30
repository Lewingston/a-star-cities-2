
#include "AStarWindow.h"
#include "../MapLoader/LoadingWindow.h"
#include "../MapLoader/MapLoader.h"
#include "../MapParser/MapParser.h"
#include "../MapUtilities/Geometry.h"
#include "../MapRenderer/NodeRenderer.h"
#include "../MapRenderer/Color.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/System/Angle.hpp>

#include <iostream>
#include <thread>

using namespace asc2;

AStarWindow::AStarWindow(uint32_t width, uint32_t height, const std::string& title) :
    Window(width, height, title),
    overlay(sf::Vector2u(width, height)),
    solver(overlay, nullptr) {

    showMapCenter = false;
    showMapBorder = false;

    overlay.setFadeSpeed(fadeSpeed);
}

void AStarWindow::show(MapLoader& mapLoader, const RenderConfig& config) {

    LoadingWindow loadingWindow(window);

    map = std::make_shared<Map>();

    mapLoader.startLoader({&loadingWindow, map});

    loadingWindow.show();

    mapLoader.join();

    solver.setMap(map);
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
    timer = std::chrono::steady_clock::now();
}

void AStarWindow::onResize(uint32_t newWidth, uint32_t newHeight) {

    Window::onResize(newWidth, newHeight);

    resetOverlay();
    timer = std::chrono::steady_clock::now();
}

void AStarWindow::applyNewView(sf::Vector2f center, sf::Vector2f size, float rotation) {
    
    const float windowWidth  = static_cast<float>(window.getSize().x);
    const float windowHeight = static_cast<float>(window.getSize().y);

    setView(center, size, rotation);

    const float windowRatio = std::abs(windowWidth / windowHeight);
    const float mapRatio = std::abs(size.x / size.y);
    const bool moreWide = mapRatio > windowRatio;

    const sf::Vector2u resolution = {
        moreWide ? window.getSize().x : static_cast<uint32_t>(static_cast<float>(window.getSize().x) * mapRatio / windowRatio),
        moreWide ? static_cast<uint32_t>(static_cast<float>(window.getSize().y) * windowRatio / mapRatio) : window.getSize().y 
    };

    overlay.setResolution(resolution);
    overlay.setPosition(center);
    overlay.setSize(size);
    overlay.setRotation(rotation);

    sf::View view = window.getView();
    view.setCenter(center);
    view.setSize(size);
    overlay.setView(view);

    currentOverlayBorders.update(center, size, rotation);

    overlay.clear();
    solver.selectStartAndEndNode();
}

void AStarWindow::resetOverlay() {

    const sf::Vector2f center {
        static_cast<float>(map->getCenter().first),
        static_cast<float>(-map->getCenter().second)
    };

    const sf::Vector2f size = {
        static_cast<float>(map->getSize().first),
        static_cast<float>(map->getSize().second)
    };

    applyNewView(center, size, 0.0f);
}

void AStarWindow::draw() {

    Window::draw();

    if (drawOverlayBoundsActive)
        currentOverlayBorders.draw(window);

    if (redrawMode)
        newOverlayBorders.draw(window);

    if (!solver.isSolved()) {

        // wait a short time before starting the first animation
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - timer) >= std::chrono::seconds(2)) {
            solver.doStepAndDraw(animationSpeed * speedMultiplier, roadPercentage / 100.0f * speedMultiplier);
        } else {
            solver.drawStartAndEndPoint(overlay.getCurrentTexture());
        }

        if (solver.isSolved()) {
            solver.drawSolution();
            timer = std::chrono::steady_clock::now();
        }
    // wait a view seconds before starting the next animation
    } else if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - timer) >= std::chrono::seconds(6)) {

        solver.reset();
    }

    overlay.draw(window);
    overlay.flip();
}

void AStarWindow::drawImGui() {

    //ImGui::ShowDemoWindow();

    if (showInfoOverlay)
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

        ImGui::Checkbox("Show Info Overlay", &showInfoOverlay);
        ImGui::Checkbox("Show map center", &showMapCenter);
        ImGui::Checkbox("Show map border", &showMapBorder);
        ImGui::Checkbox("Show overlay border", &drawOverlayBoundsActive);

        if (ImGui::Button("Reset View Port to Map Border")) {
            resetView();
        }

        if (ImGui::Button("Reset View Port to Render Overlay")) {
            setView(currentOverlayBorders);
        }

        ImGui::SliderFloat("Map rotation", &rotationInput, -180.0f, 180.0f);

        ImGui::InputScalar("Map rotation input:", ImGuiDataType_Float, &rotationInput);

        if (mapRotation > rotationInput || mapRotation < rotationInput) {
            setRotation(rotationInput);
        }

        if (!redrawMode) {
            if (ImGui::Button("Redraw Overlay")) {
                redrawMode = true;
            }
            if (ImGui::Button("Reset Overlay")) {
                resetOverlay();
            }
        } else {
            if (ImGui::Button("Ok")) {
                applyNewView(newOverlayBorders.getCenter(), newOverlayBorders.getSize(), newOverlayBorders.getRotation());
                redrawMode = false;
            }
        }
    }

    if (ImGui::CollapsingHeader("Simulation Controls")) {

        ImGui::SliderFloat("Speed", &animationSpeed, 0.0f, 1000.0f);
        ImGui::Text("Number of roads per frame\nas %% of nodes in the open list");
        ImGui::SliderFloat("%", &roadPercentage, 0.0f, 100.0f);
        ImGui::SliderFloat("Speed Multiplier", &speedMultiplier, 0.0f, 100.0f);

        if (ImGui::Button("Reset Speed Settings")) {
            animationSpeed = 75.0f;
            roadPercentage = 100.0f / 15.0f;
            speedMultiplier = 1.0f;
        }

        if (ImGui::Button("Restart")) {
            solver.restart();
        }

        if (ImGui::Button("Reset")) {
            solver.reset();
        }

        ImGui::InputInt("Fade speed", &fadeSpeed, 1, 100);
        overlay.setFadeSpeed(fadeSpeed);
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

    const sf::Vector2f center = {
        static_cast<float>(map->getCenter().first),
        static_cast<float>(-map->getCenter().second)
    };

    const sf::Vector2f size = {
        static_cast<float>(map->getSize().first),
        static_cast<float>(map->getSize().second)
    };

    setView(center, size, 0.0f);
}

void AStarWindow::setView(const BorderDrawer& borders) {

    setView(borders.getCenter(), borders.getSize(), borders.getRotation());
}

void AStarWindow::setView(sf::Vector2f center, sf::Vector2f size, float rotation) {

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

    mapRotation = rotation;
    rotationInput = rotation;
}

std::vector<std::reference_wrapper<const Intersection>> AStarWindow::getIntersectionsInArea(const std::array<sf::Vector2f, 4>& area) const {

    std::vector<std::reference_wrapper<const Intersection>> inArea;

    const Geometry::Rectangle rectangle = { area };

    for (const Intersection& inter : map->getMainIntersections()) {

        const sf::Vector2f pos { 
            static_cast<float>(inter.getNode().lon),
            static_cast<float>(-inter.getNode().lat) };

        if (Geometry::pointInRectangle(pos, rectangle)) {
            inArea.emplace_back(inter);
        }
    }

    return inArea;
}