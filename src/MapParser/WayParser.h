#pragma once

#include "MapParserConfig.h"
#include "NodeParser.h"
#include "../Map/MapFeatureType.h"

#include <nlohmann/json.hpp>

#include <limits>

namespace asc2 {

    class WayParser {

        using json = nlohmann::json;

        using Node = NodeParser::Node;

        public:

            struct Way {

                const uint64_t        id;
                const MapFeatureType  type;
                const json&           data;
                std::vector<uint64_t> nodeIds;
                bool isComplete       = false;
                bool isPartOfRelation = false;
            };

            WayParser(const MapParserConfig& config);

            bool parse(const json& data);

            [[nodiscard]] std::size_t getWayCount() const noexcept { return ways.size(); }
            [[nodiscard]] std::map<uint64_t, Way>& getWays() noexcept { return ways; }

        private:

            std::map<uint64_t, Way> ways;

            MapParserConfig config;

    };
}