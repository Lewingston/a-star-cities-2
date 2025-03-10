#pragma once

#include "NodeParser.h"

#include <nlohmann/json.hpp>

namespace asc2 {

    class WayParser {

        using json = nlohmann::json;

        using Node = NodeParser::Node;

        public:

            struct Way {
                const uint64_t id;
                const json& data;
                std::vector<uint64_t> nodeIds;
                bool isComplete = false;
            };

            bool parse(const json& data);

            [[nodiscard]] std::size_t getWayCount() const noexcept { return ways.size(); }
            [[nodiscard]] std::map<uint64_t, Way>& getWays() noexcept { return ways; }

        private:

            std::map<uint64_t, Way> ways;

    };
}