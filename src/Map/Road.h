#pragma once

#include "RoadType.h"

namespace asc2 {

    class Way;

    class Road {

        public:

            Road(RoadType type, const Way& way) : 
                type(type),
                way(way) {}

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

        private:

            RoadType type;
            const Way& way;
    };
}