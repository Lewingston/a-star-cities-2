#pragma once

#include "Node.h"
#include "Way.h"

#include <map>

namespace asc2 {

    class Map {

        public:

            Map() = default;

            void addNode(const Node& node);
            
            void addWay(const Way& way);

            [[nodiscard]] std::vector<std::reference_wrapper<const Node>> getNodes(
                const std::vector<uint64_t>& nodeIds);

            [[nodiscard]] const std::map<uint64_t, Node>& getAllNodes() const noexcept;
            [[nodiscard]] const std::map<uint64_t, Way>&  getAllWays()  const noexcept;

        private:

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;

    };
}