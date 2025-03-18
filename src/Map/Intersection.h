#pragma once

#include <vector>

namespace asc2 {

    class Node;
    class Road;

    class Intersection {

        public:

            Intersection(const Node& node);

            void addRoad(const Road& road);

        private:

            const Node& node;

            std::vector<std::reference_wrapper<const Road>> roads;
    };
}