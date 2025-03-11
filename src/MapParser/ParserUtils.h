#pragma once

#include "../Map/MapFeatureType.h"

#include <nlohmann/json.hpp>

namespace asc2 {

    using json = nlohmann::json;

    class ParserUtils {

        public:

            [[nodiscard]] static std::vector<uint64_t> getIdsFromArray(const json& array);

            [[nodiscard]] static MapFeatureType getType(const json& data, const std::set<MapFeatureType>& allowedTypes);
    };
}