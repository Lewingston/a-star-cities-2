
#include "MapLoader.h"
#include "LoadingWindow.h"
#include "../MapParser/MapParser.h"
#include "../MapUtilities/NetworkFinder.h"

using namespace asc2;

void MapLoader::startLoader(const LoaderParams& params) {

    loaderParams = params;
    if (loaderParams.filePath.empty())
        loaderParams.filePath = filePath;

    loadingThread = std::thread([&]() {
        loadingThreadFunction(loaderParams);
    });
}

void MapLoader::join() {

    loadingThread.join();
}

void MapLoader::loadingThreadFunction(const LoaderParams& params) {

    uint32_t currentStep = 0;
    const uint32_t totalSteps = static_cast<uint32_t>(loadingSteps.size());
    params.window->setProgress(currentStep, totalSteps);

    for (const auto& step : loadingSteps) {
        if (!step(params)) {
            params.window->addStatusMessage("Error!");
            return;
        }
        params.window->setProgress(++currentStep, totalSteps);
    }

    params.window->finishedLoading();
}

void MapLoader::addStep(LoaderStep step) {

    loadingSteps.push_back(step);
}

void MapLoader::setDefualtLoad() {

    // load file
    addStep([&mapParser = mapParser, &parserConfig = parserConfig](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Loading file: " + params.filePath);

        try {
            mapParser = std::make_shared<MapParser>(params.filePath, parserConfig);
        } catch (const std::exception& e) {
            params.window->addStatusMessage("Failed to load file: " + params.filePath);
            return false;
        }

        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("File loaded: " + params.filePath);

        return true;
    });

    // parse file
    addStep([&mapParser = mapParser](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Parsing map...");

        if (!mapParser->parse(params.map)) {
            return false;
        }

        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Map parsed.");
        params.window->addStatusMessage("Node count: " + std::to_string(params.map->getAllNodes().size()));
        params.window->addStatusMessage("Way count: " + std::to_string(params.map->getAllWays().size()));

        return true;
    });
}

void MapLoader::setAStarLoad() {

    setDefualtLoad();

    //  remove intersections with only one road
    addStep([](const LoaderParams& params) -> bool {

        const std::size_t interBefore = params.map->getAllIntersections().size();
        params.window->addStatusMessage("Removing with only one road...");

        params.map->removeUnnecessaryIntersections();

        const std::size_t interAfter = params.map->getAllIntersections().size();
        const std::size_t count = interBefore - interAfter;
        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Removed " + std::to_string(count) + " intersections with only one road");

        return true;
    });

    // split roads on intersections
    addStep([](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Split roads on intersections (This might take some time)");
        const std::size_t roadsOld = params.map->getAllRoads().size();

        params.map->splitRoadsOnIntersections();

        const std::size_t roadsNew = params.map->getAllRoads().size();
        const std::size_t count = roadsNew - roadsOld;
        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Splitted roads on intersections. " + std::to_string(count) + " new roads added");

        return true;
    });

    // fuse roads
    addStep([](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Fuse roads on intersections...");
        const std::size_t interOld = params.map->getAllIntersections().size();

        params.map->fuseRoads();

        const std::size_t newInter = params.map->getAllIntersections().size();
        const std::size_t count = interOld - newInter;
        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Roads fused. Removed " + std::to_string(count) + " intersections");

        return true;
    });

    // find networks
    addStep([&networkFinder = networkFinder](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Find networks...");
        
        networkFinder = std::make_shared<NetworkFinder>(*params.map);
        const std::size_t count = networkFinder->findNetworks().size();
        
        params.window->removeLastStatusMessage();
        if (count > 1)
            params.window->addStatusMessage("Found " + std::to_string(count) + " networks");
        else
            params.window->addStatusMessage("Found " + std::to_string(count) + " network");

        return true;
    });

    // remove unconnected networks
    addStep([&networkFinder = networkFinder](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Removing unconnected networks...");
        const std::size_t roadsBefore = params.map->getAllRoads().size();

        networkFinder->removeUnconnectedNetworks();

        const std::size_t roadsAfter = params.map->getAllRoads().size();
        const std::size_t count = roadsBefore - roadsAfter;
        const float percentage = static_cast<float>(count) / static_cast<float>(roadsBefore) * 100.0f;
        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Removed " + std::to_string(count) + " roads (" + std::to_string(percentage) + "%)");

        return true;
    });

    // add intersections to end points
    addStep([](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Adding intersectiosn to the ends of roads...");

        params.map->addIntersectionsToEndPoints();

        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Added intersections to end of raods");

        return true;
    });

    // segmentate roads
    addStep([](const LoaderParams& params) -> bool {

        params.window->addStatusMessage("Segmenting roads...");

        params.map->segmentRoads(50.0f);
        
        params.window->removeLastStatusMessage();
        params.window->addStatusMessage("Roads are segmented");

        return true;
    });
}