#pragma once

#include <nlohmann/json.hpp>

namespace asc2 {

    using json = nlohmann::json;

    class NodeParser {

        public:

            struct Node {
                const uint64_t id;
                const double   lon;
                const double   lat;
                const json&    data;
            };

            //[[nodiscard]] static Node parse(const json& data);

            NodeParser() = default;

            bool parse(const json& data);

            [[nodiscard]] std::size_t getNodeCount() const noexcept { return nodes.size(); }
            [[nodiscard]] const std::map<uint64_t, Node>& getNodes() const noexcept { return nodes; }

            /*[[nodiscard]] static std::vector<std::reference_wrapper<const Node>> getNodes(
                const std::map<uint64_t, Node>& nodeMap, const std::vector<uint64_t>& ids);*/

        private:

            std::map<uint64_t, Node> nodes;
    };
}