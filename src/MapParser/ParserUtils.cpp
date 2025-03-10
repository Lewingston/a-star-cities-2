
#include "ParserUtils.h"

#include <iostream>

using namespace asc2;

std::vector<uint64_t> ParserUtils::getIdsFromArray(const json& array) {

    if (!array.is_array()) {
        std::cerr << "Failed to get array of ids. Element is not an array!\n";
        return {};
    }

    std::vector<uint64_t> ids;
    ids.reserve(array.size());

    for (const auto& id : array) {
        if (!id.is_number_unsigned()) {
            std::cerr << "Failed to get array of ids. Array contains invalid element.\n";
            return ids;
        }
        ids.emplace_back(id.get<uint64_t>());
    }

    return ids;
}