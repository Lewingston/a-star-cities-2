#pragma once

#include "Node.h"

#include <vector>

namespace asc2 {

    class Node;
    class Road;

    class Intersection {

        public:

            Intersection(const Node& node);

            void addRoad(Road& road);

            void removeRoad(const Road& road);

            [[nodiscard]] uint64_t getId() const noexcept { return node.id; }

            [[nodiscard]] const Node& getNode() const noexcept { return node; }

            [[nodiscard]] const std::vector<std::reference_wrapper<Road>>& getRoads() const noexcept { return roads; }

            bool operator== (const Intersection& inter) const { return inter.getNode().id == node.id; }

        private:

            const Node& node;

            std::vector<std::reference_wrapper<Road>> roads;
    };
}