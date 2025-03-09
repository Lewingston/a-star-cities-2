#pragma once

#include <cstdint>
#include <utility>

namespace asc2 {

    struct Node {

        const uint64_t id;
        const double lon;
        const double lat;
        double x = 0.0;
        double y = 0.0;

        void setProjectedPosition(std::pair<double, double> pos) {
            x = pos.first;
            y = pos.second;
        }
    };
}