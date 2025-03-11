#pragma once

#include <nlohmann/json.hpp>

namespace asc2 {

    class NodeParser {

        public:

            using json = nlohmann::json;

            struct Node {
                const uint64_t id;
                const double   lon;
                const double   lat;
                const json&    data;
            };

            NodeParser() = default;

            bool parse(const json& data);

            [[nodiscard]] std::size_t getNodeCount() const noexcept { return nodes.size(); }
            [[nodiscard]] const std::map<uint64_t, Node>& getNodes() const noexcept { return nodes; }

        private:

            std::map<uint64_t, Node> nodes;
    };
}