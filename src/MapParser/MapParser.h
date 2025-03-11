#pragma once

#include "MapParserConfig.h"
#include "NodeParser.h"
#include "WayParser.h"
#include "RelationParser.h"
#include "../Map/Map.h"

#include <nlohmann/json.hpp>

#include <string>
#include <memory>

namespace asc2 {
    
    using json = nlohmann::json;

    class MapParser{

        public:

            using Node = NodeParser::Node;
            using Way  = WayParser::Way;

            MapParser(const std::string& filePath, const MapParserConfig& config);

            std::unique_ptr<Map> parse();

        private:

            struct Relation {
                const uint64_t id;
                const json     data;
                bool isComplete = false;
            };

            void loadFromFile(const std::string& filePath);

            bool parseElements(const json& data);
            bool parseElement(const json& element);

            void addNodesToMap();
            void addWaysToMap();

            void parseWayTypes();
            [[nodiscard]] bool wayIsHighway(const Way& way) const;
            [[nodiscard]] bool wayIsBuilding(const Way& way) const;
            void parseHighway(const Way& way);
            void parseBuilding(const Way& way);

            MapParserConfig config;

            json data;

            NodeParser     nodeParser;
            WayParser      wayParser;
            RelationParser relationParser;

            std::unique_ptr<Map> map;
    };
}