#pragma once

#include "BuildingType.h"

namespace asc2 {

    class Building {

        public:

            Building(BuildingType type, const Way& way) :
                type(type),
                way(way) {}

            [[nodiscard]] const Way& getWay() const noexcept { return way; }

        private:

            BuildingType type;
            const Way& way;
    };
}