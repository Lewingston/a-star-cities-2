#pragma once

#include <cstdint>
#include <vector>

namespace asc2 {

    class Node;
    class Intersection;

    class Way {

        public:

            Way(uint64_t id, const std::vector<std::reference_wrapper<const Node>>& nodes, bool isComplete);

            [[nodiscard]] uint64_t getId() const noexcept { return id; }

            [[nodiscard]] const std::vector<std::reference_wrapper<const Node>>& getNodes() const noexcept { return nodes; }

            [[nodiscard]] bool isComplete() const noexcept { return complete; }

            [[nodiscard]] std::size_t getNodeCount() const noexcept { return nodes.size(); }
            [[nodiscard]] std::size_t getLineCount() const noexcept { return nodes.size() - 1; }

        private:

            uint64_t id;
            std::vector<std::reference_wrapper<const Node>> nodes;
            bool complete = false;
    };
}