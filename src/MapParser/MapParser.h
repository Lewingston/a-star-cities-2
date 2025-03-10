#pragma once

#include "NodeParser.h"
#include "WayParser.h"
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

            MapParser(const std::string& filePath);

            std::unique_ptr<Map> parse();

        private:

            struct Relation {
                const uint64_t id;
                const json     data;
                bool isComplete = false;
            };

            void loadFromFile(const std::string& filePath);

            void parseElements(const json& data);

            void addNodesToMap();
            void addWaysToMap();

            void parseRelation(const json& data);

            void constructRelations();
            void constructRelation(Relation& relation) const;

            void parseWayTypes();
            [[nodiscard]] bool wayIsHighway(const Way& way) const;
            [[nodiscard]] bool wayIsBuilding(const Way& way) const;
            void parseHighway(const Way& way);
            void parseBuilding(const Way& way);

            json data;

            NodeParser nodeParser;
            WayParser  wayParser;

            std::map<uint64_t, Relation> relations;

            std::unique_ptr<Map> map;
    };
}