
#include "WayParser.h"
#include "ParserUtils.h"

#include <iostream>

using namespace asc2;

bool WayParser::parse(const json& data) {

    const uint64_t id = data["id"];

    if (!data.contains("nodes") || data["nodes"].size() == 0) {
        std::cout << "Way has no nodes:\n" << data.dump() << '\n';
        return true;
    }

    const auto [iter, success] = ways.emplace(id, Way {
        .id         = id,
        .data       = data,
        .nodeIds    = ParserUtils::getIdsFromArray(data["nodes"]),
        .isComplete = false
    });

    if (!success) {
        std::cerr << "Duplicate way ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }

    return success;
}