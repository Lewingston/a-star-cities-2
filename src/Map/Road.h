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
                std::vector<std::reference_wrapper<const Node>> nodes {};
                std::vector<std::reference_wrapper<Intersection>> intersections {};
            };

            Road(RoadType type, const Way& way) : 
                type(type),
                way(way) {}

            [[nodiscard]] uint64_t getId() const noexcept { return way.getId(); }

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

            [[nodiscard]] RoadType getType() const noexcept { return type; }

            [[nodiscard]] const std::vector<std::reference_wrapper<Intersection>>& getIntersections() const noexcept { return intersections; }

            void addIntersection(Intersection& intersection);

            void removeIntersection(const Intersection& intersection);

            std::vector<NewRoadData> splitRoadOnIntersections() const;

            NewRoadData fuse(const Road& road) const;

            void removeFromIntersections();

        private:

            RoadType type;

            const Way& way;

            std::vector<std::reference_wrapper<Intersection>> intersections;
    };
}