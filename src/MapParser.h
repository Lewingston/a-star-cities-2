#pragma once

#include "Map/Map.h"

#include <nlohmann/json.hpp>

#include <string>
#include <memory>

using json = nlohmann::json;

namespace asc2 {

    class MapParser{

        public:

            MapParser() = default;

            void loadFromFile(const std::string& filePath);
            void parse();

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

            void parseNodes(const json& data);

            void parseElements(const json& data);
            void constructWays();
            void constructWay(Way& way) const;

            void parseNode(const json& data);
            void parseWay(const json& data);

            [[nodiscard]] static std::vector<uint64_t> getIdArray(const json& json);

            json data;

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;

            std::unique_ptr<Map> map;
    };
}