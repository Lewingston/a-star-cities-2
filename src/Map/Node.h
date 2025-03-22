#pragma once

#include <cstdint>
#include <utility>

namespace asc2 {

    struct Node {

        const uint64_t id;
        const double lon;
        const double lat;

        float distance(const Node& node) const;

        std::pair<double, double> pointBetween(const Node& node, float distance) const;
    };
}