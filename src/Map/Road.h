#pragma once

#include "RoadType.h"

#include <vector>

namespace asc2 {

    class Way;
    class Intersection;

    class Road {

        public:

            Road(RoadType type, const Way& way) : 
                type(type),
                way(way) {}

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

            void addIntersection(const Intersection& intersection);

        private:

            RoadType type;
            const Way& way;

            std::vector<std::reference_wrapper<const Intersection>> intersections;
    };
}