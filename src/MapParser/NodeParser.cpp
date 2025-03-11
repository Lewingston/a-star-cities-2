
#include "NodeParser.h"

#include <iostream>

using namespace asc2;

bool NodeParser::parse(const json& data) {

    const uint64_t id  = data["id"];
    const double   lon = data["lon"];
    const double   lat = data["lat"];

    const auto [iter, success] = nodes.emplace(id, Node {
        .id   = id,
        .lon  = lon,
        .lat  = lat,
        .data = data
    });
    if (!success) {
        std::cerr << "Duplicate node ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }
    return success;
}