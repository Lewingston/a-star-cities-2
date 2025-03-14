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

    class MapParser{

        public:

            using json = nlohmann::json;

            using Node = NodeParser::Node;
            using Way  = WayParser::Way;

            MapParser(const std::string& filePath, const MapParserConfig& config);

            std::unique_ptr<Map> parse();

        private:

            void loadFromFile(const std::string& filePath);

            bool parseElements(const json& data);
            bool parseElement(const json& element);

            void addNodesToMap();
            void addWaysToMap();

            void parseWayTypes();
            void parseRelationTypes();

            void addBuilding(const RelationParser::Relation& relation);

            MapParserConfig config;

            json data;

            NodeParser     nodeParser;
            WayParser      wayParser;
            RelationParser relationParser;

            std::unique_ptr<Map> map;
    };
}