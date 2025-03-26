
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

AStarWindow::AStarWindow(uint32_t width, uint32_t height, const std::string& title)
    : Window(width, height, title) {

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

        if (ImGui::Button("Reset View Port")) {
            resetView();
        }

        ImGui::SliderFloat("Map rotation", &rotationInput, -180.0f, 180.0f);

        ImGui::InputScalar("Map rotation input:", ImGuiDataType_Float, &rotationInput);

        if (mapRotation != rotationInput) {
            setRotation(rotationInput);
        }
    }

    ImGui::End();
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