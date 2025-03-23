
#include "AStarWindow.h"
#include "../MapLoader/LoadingWindow.h"
#include "../MapParser/MapParser.h"

#include <iostream>
#include <thread>

using namespace asc2;

AStarWindow::AStarWindow(uint32_t width, uint32_t height, const std::string& title)
    : Window(width, height, title) {

}

/*void AStarWindow::show() {

    loadMap(mapFileName);

    Window::show();
}*/

void AStarWindow::loadMap(const std::string& filePath) {

    LoadingWindow loadingWindow(getWindow());

    std::unique_ptr<Map> map;

    //std::thread loadingThread(loadThread, filePath, loadingWindow);
    std::thread loadingThread([&]() {
        loadThread(filePath, loadingWindow, map);
    });

    loadingWindow.show();

    loadingThread.join();

    const RenderConfig renderConfig {
        .displayIncomplete = false,
        .buildingLineMode  = false,
        .backgroundColor   = sf::Color::Black,
        .roadColor         = sf::Color(80, 0, 0),
        .roadColors        = {
            { RoadType::LIVING_STREET,  sf::Color(16, 16, 16) },
            { RoadType::RESIDENTIAL,    sf::Color(18, 18, 18) },
            { RoadType::UNCLASSIFIED,   sf::Color(20, 20, 20) },
            { RoadType::TERTIARY,       sf::Color(22, 22, 22) },
            { RoadType::TERTIARY_LINK,  sf::Color(22, 22, 22) },
            { RoadType::SECONDARY,      sf::Color(24, 24, 24) },
            { RoadType::SECONDARY_LINK, sf::Color(24, 24, 24) },
            { RoadType::PRIMARY,        sf::Color(26, 26, 26) },
            { RoadType::PRIMARY_LINK,   sf::Color(26, 26, 26) },
            { RoadType::TRUNK,          sf::Color(28, 28, 28) },
            { RoadType::TRUNK_LINK,     sf::Color(28, 28, 28) },
            { RoadType::MOTORWAY,       sf::Color(30, 30, 30) },
            { RoadType::MOTORWAY_LINK,  sf::Color(30, 30, 30) }
        }
    };

    if (map != nullptr) {
        renderMap(*map, renderConfig);
    }
}

void AStarWindow::loadThread(const std::string& filePath, LoadingWindow& window, std::unique_ptr<Map>& map) {

    static constexpr float ROAD_SEGMENT_LENGTH = 50.0f;

    const uint32_t totalSteps = 9;
    uint32_t currentStep = 0;

    const MapParserConfig parserConfig {
        .mapFeatures = {
            MapFeatureType::HIGHWAY,
        },
        .includeIncompleteWays = false
    };

    // load file
    window.addStatusMessage("Loading file: " + filePath);
    MapParser parser(filePath, parserConfig);
    window.removeLastStatusMessage();
    window.addStatusMessage("File loaded: " + filePath);
    window.setProgress(++currentStep, totalSteps);

    // parse file
    window.addStatusMessage("Parsing map...");
    //map = parser.parse();
    if (map == nullptr) {
        window.addStatusMessage("Failed to parse map!");
        std::cerr << "Failed to parse map: " << filePath << '\n';
    }
    window.removeLastStatusMessage();
    window.addStatusMessage("Map parsed. Road count: " + std::to_string(map->getAllRoads().size()) + " - Node count: " + std::to_string(map->getAllNodes().size()));
    window.setProgress(++currentStep, totalSteps);

    { // remove intersections with only one road
    const std::size_t interBefore = map->getAllIntersections().size();
    window.addStatusMessage("Removing with only one road...");
    map->removeUnnecessaryIntersections();
    const std::size_t interAfter = map->getAllIntersections().size();
    const std::size_t count = interBefore - interAfter;
    window.removeLastStatusMessage();
    window.addStatusMessage("Removed " + std::to_string(count) + " intersections with only one road");
    window.setProgress(++currentStep, totalSteps);
    }

    { // Split roads on intersections
    window.addStatusMessage("Split roads on intersections (This might take some time)");
    const std::size_t roadsOld = map->getAllRoads().size();
    map->splitRoadsOnIntersections();
    const std::size_t roadsNew = map->getAllRoads().size();
    const std::size_t count = roadsNew - roadsOld;
    window.removeLastStatusMessage();
    window.addStatusMessage("Splitted roads on intersections. " + std::to_string(count) + " new roads added");
    window.setProgress(++currentStep, totalSteps);
    }

    { // fuse roads
    window.addStatusMessage("Fuse roads on intersections...");
    const std::size_t interOld = map->getAllIntersections().size();
    map->fuseRoads();
    const std::size_t newInter = map->getAllIntersections().size();
    const std::size_t count = interOld - newInter;
    window.removeLastStatusMessage();
    window.addStatusMessage("Roads fused. Removed " + std::to_string(count) + " intersections");
    window.setProgress(++currentStep, totalSteps);
    }

    NetworkFinder networkFinder(*map);

    { // find networks
    window.addStatusMessage("Find networks...");
    const std::size_t count = networkFinder.findNetworks().size();
    window.removeLastStatusMessage();
    if (count > 1)
        window.addStatusMessage("Found " + std::to_string(count) + " networks");
    else
        window.addStatusMessage("Found " + std::to_string(count) + " network");
    window.setProgress(++currentStep, totalSteps);
    }

    { // remove unconnected networks
    window.addStatusMessage("Removing unconnected networks...");
    const std::size_t roadsBefore = map->getAllRoads().size();
    networkFinder.removeUnconnectedNetworks();
    const std::size_t roadsAfter = map->getAllRoads().size();
    const std::size_t count = roadsBefore - roadsAfter;
    const float percentage = static_cast<float>(count) / static_cast<float>(roadsBefore) * 100.0f;
    window.removeLastStatusMessage();
    window.addStatusMessage("Removed " + std::to_string(count) + " roads (" + std::to_string(percentage) + "%)");
    window.setProgress(++currentStep, totalSteps);
    }

    { // add intersections to end points
    window.addStatusMessage("Adding intersectiosn to the ends of roads...");
    map->addIntersectionsToEndPoints();
    window.removeLastStatusMessage();
    window.addStatusMessage("Added intersections to end of raods");
    window.setProgress(++currentStep, totalSteps);
    }

    { // segmentate roads
    window.addStatusMessage("Segmenting roads...");
    map->segmentRoads(ROAD_SEGMENT_LENGTH);
    window.removeLastStatusMessage();
    window.addStatusMessage("Roads are segmented");
    window.setProgress(++currentStep, totalSteps);
    }

    window.finishedLoading();
}