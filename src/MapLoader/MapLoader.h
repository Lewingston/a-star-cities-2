#pragma once

#include "../MapParser/MapParserConfig.h"

#include <thread>
#include <string>
#include <memory>
#include <functional>

namespace asc2 {

    class LoadingWindow;
    class MapParser;
    class Map;
    class NetworkFinder;

    class MapLoader {

        public:

            struct LoaderParams {
                LoadingWindow* window = nullptr;
                std::shared_ptr<Map> map;
                std::string filePath {};
            };

            using LoaderStep = std::function<bool(const LoaderParams& params)>;

            MapLoader(const std::string& filePath, const MapParserConfig& parserConfig) :
                filePath(filePath),
                parserConfig(parserConfig) {}

            void startLoader(const LoaderParams& params);

            void join();

            void addStep(LoaderStep step);

            void setDefualtLoad();

            void setAStarLoad();

        private:

            void loadingThreadFunction(const LoaderParams& params);

            std::thread loadingThread;

            std::string filePath;

            LoaderParams loaderParams{};

            MapParserConfig parserConfig;

            std::shared_ptr<MapParser> mapParser;

            std::shared_ptr<NetworkFinder> networkFinder;

            std::vector<LoaderStep> loadingSteps;
    };
}