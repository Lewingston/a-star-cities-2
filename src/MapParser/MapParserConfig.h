#pragma once

#include "../Map/MapFeatureType.h"

#include <vector>

namespace asc2 {

    class MapParserConfig {

        public:

            std::set<MapFeatureType> mapFeatures;

            bool includeIncompleteWays = false;

        private:
    };
}