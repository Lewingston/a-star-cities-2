#pragma once

#include "BuildingType.h"
#include "Way.h"

namespace asc2 {

    class Building {

        public:

            Building(BuildingType type, const Way& way);

            Building(BuildingType type,
                     uint64_t buildingId,
                     const std::vector<std::reference_wrapper<const Way>>& outerWays,
                     const std::vector<std::reference_wrapper<const Way>>& innerWays);

            [[nodiscard]] const std::vector<std::reference_wrapper<const Way>>& getOuterWays() const noexcept {
                return outerWays;
            }

            [[nodiscard]] const std::vector<std::reference_wrapper<const Way>>& getInnerWays() const noexcept {
                return innerWays;
            }

        private:

            BuildingType type;
            uint64_t id;
            bool isRelation;

            std::vector<std::reference_wrapper<const Way>> outerWays;
            std::vector<std::reference_wrapper<const Way>> innerWays;
    };
}