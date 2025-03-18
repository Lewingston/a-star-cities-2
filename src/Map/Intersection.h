#pragma once

#include <vector>

namespace asc2 {

    class Node;
    class Road;

    class Intersection {

        public:

            Intersection(const Node& node);

            void addRoad(const Road& road);

            [[nodiscard]] const Node& getNode() const noexcept { return node; }

            [[nodiscard]] const std::vector<std::reference_wrapper<const Road>>& getRoads() const noexcept { return roads; }

        private:

            const Node& node;

            std::vector<std::reference_wrapper<const Road>> roads;
    };
}