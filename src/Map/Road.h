#pragma once

#include "RoadType.h"
#include "Way.h"

#include <vector>

namespace asc2 {

    class Way;
    class Intersection;

    class Road {

        public:

            struct NewRoadData {
                RoadType type;
                std::vector<std::reference_wrapper<Intersection>> intersections;
            };

            Road(RoadType type, const Way& way) : 
                type(type),
                way(way) {}

            [[nodiscard]] uint64_t getId() const noexcept { return way.getId(); }

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

            void addIntersection(Intersection& intersection);

            std::vector<NewRoadData> splitRoadOnIntersections() const;

            void removeFromIntersections();

        private:

            RoadType type;
            const Way& way;

            std::vector<std::reference_wrapper<Intersection>> intersections;
    };
}