#pragma once

#include <cstdint>
#include <vector>

namespace asc2 {

    class Node;

    struct Way {

        uint64_t id;
        std::vector<std::reference_wrapper<const Node>> nodes;
        bool isComplete = false;
    };
}