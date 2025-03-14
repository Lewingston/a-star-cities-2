#pragma once

#include "../Map/RoadType.h"
#include "WayParser.h"

namespace asc2 {

    class RoadParser {

        public:

            [[nodiscard]] static RoadType getRoadType(const WayParser::Way& way);

        private:
    };
}