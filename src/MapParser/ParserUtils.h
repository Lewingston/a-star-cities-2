#pragma once

#include <nlohmann/json.hpp>

namespace asc2 {

    using json = nlohmann::json;

    class ParserUtils {

        public:

            [[nodiscard]] static std::vector<uint64_t> getIdsFromArray(const json& array);
    };
}