#pragma once

#include <cstdint>
#include <vector>

namespace asc2 {

    class Node;

    struct Way {

        uint64_t id;
        std::vector<std::reference_wrapper<const Node>> nodes;
        bool isComplete = false;

        [[nodiscard]] std::size_t getNodeCount() const noexcept { return nodes.size(); }
        [[nodiscard]] std::size_t getLineCount() const noexcept { return nodes.size() - 1; }
    };
}