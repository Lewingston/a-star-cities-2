#pragma once

#include "MapParserConfig.h"
#include "NodeParser.h"
#include "WayParser.h"
#include "../Map/MapFeatureType.h"

#include <nlohmann/json.hpp>

namespace asc2 {

    class RelationParser {

        public:

            using json = nlohmann::json;

            using Way  = WayParser::Way;
            using Node = NodeParser::Node;

            struct Relation {

                const uint64_t id;
                const MapFeatureType type;
                const json& data;
                std::vector<std::reference_wrapper<Way>> outerWays {};
                std::vector<std::reference_wrapper<Way>> innerWays {};
                bool isComplete;
            };

            RelationParser(const MapParserConfig& config);

            bool parse(const json& data);

            void constructRelations(std::map<uint64_t, Way>& wayMap);
            void constructRelation(Relation& relation, std::map<uint64_t, Way>& wayMap);

            [[nodiscard]] std::size_t getRelationCount() const noexcept { return relations.size(); }
            [[nodiscard]] std::map<uint64_t, Relation>& getRelations() noexcept { return relations; }

        private:

            std::map<uint64_t, Relation> relations;

            MapParserConfig config;

    };
}