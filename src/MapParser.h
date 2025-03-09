#pragma once

#include "Map/Map.h"

#include <nlohmann/json.hpp>

#include <string>
#include <memory>

using json = nlohmann::json;

namespace asc2 {

    class MapParser{

        public:

            MapParser(const std::string& filePath);

            std::unique_ptr<Map> parse();

        private:

            struct Node {
                const uint64_t id;
                const json     data;
            };

            struct Way  {
                const uint64_t id;
                const json     data;
                bool isComplete = false;
            };

            struct Dimensions {
                double minLon = 180.0f;
                double maxLon = 0.0f;
                double minLat = 90.0;
                double maxLat = 0.0;

                void adjust(double lon, double lat);
                std::pair<double, double> getCenter() const;
            };

            void loadFromFile(const std::string& filePath);

            void parseNodes(const json& data);

            void parseElements(const json& data);
            void constructWays();
            void constructWay(Way& way) const;

            void parseNode(const json& data);
            void parseWay(const json& data);

            void parseWayTypes();
            [[nodiscard]] bool wayIsHighway(const Way& way) const;
            void parseHighway(const Way& way);

            [[nodiscard]] static std::vector<uint64_t> getIdArray(const json& json);

            json data;

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;

            std::unique_ptr<Map> map;

            Dimensions dimensions;
    };
}