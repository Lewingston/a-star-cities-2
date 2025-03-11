
#include "RelationParser.h"
#include "ParserUtils.h"

#include <iostream>

using namespace asc2;

RelationParser::RelationParser(const MapParserConfig& config) :
    config(config) { }

bool RelationParser::parse(const json& data) {
    
    const uint64_t id = data["id"];

    const Relation relation {
        .id         = id,
        .type       = ParserUtils::getType(data, config.mapFeatures),
        .data       = data,
        .isComplete = false
    };

    const auto [iter, success] = relations.emplace(id, relation);
    if (!success) {
        std::cerr << "Duplicate relation ids:\n";
        std::cerr << iter->second.data.dump() << '\n';
        std::cerr << data.dump() << '\n';
    }

    return success;
}

void RelationParser::constructRelations(std::map<uint64_t, Way>& wayMap) {

    std::cout << "Construct relations\n";

    for (auto& [id, relation] : relations) {

        if (!config.mapFeatures.contains(relation.type))
            continue;

        constructRelation(relation, wayMap);
    }
}

void RelationParser::constructRelation(Relation& relation, std::map<uint64_t, Way>& wayMap) {

    relation.isComplete = false;

    if (!relation.data.contains("members") || relation.data["members"].size() == 0) {
        std::cout << "Relation has no members: " << relation.data.dump() << '\n';
        return;
    }

    if (!relation.data.contains("tags"))
        return;

    // Ignore all relations other than "multipolygon" for now
    if (!relation.data["tags"].contains("type") || relation.data["tags"]["type"] != "multipolygon") {
        if (relation.type == MapFeatureType::BUILDING) {
            std::cout << "Bulding relation is type other than multipolygon: " << relation.data["tags"]["type"] << '\n';
            std::cout << "Relation Id: " << relation.id << '\n';
        }
        return;
    }

    bool complete = true;

    for (const auto& element : relation.data["members"]) {

        const std::string type = element["type"];
        const std::string role = element["role"];
        const uint64_t    ref  = element["ref"];

        if (type == "node" || type == "relation")
            continue;

        if (role == "outer" || role == "inner") {

            if (auto find = wayMap.find(ref); find != wayMap.end()) {
                
                find->second.isPartOfRelation = true;

                if (role == "outer")
                    relation.outerWays.emplace_back(find->second);
                else if (role == "inner")
                    relation.innerWays.emplace_back(find->second);

            } else {
                complete = false;
            }

        } else {
            std::cout << "Unknown relation role: " << role << " in relation: " << relation.id << '\n';
        }
    }

    if (relation.outerWays.size() == 0)
        std::cerr << "Relation has no outer ways: " << relation.id << '\n';
    else if (relation.outerWays.size() > 1)
        std::cerr << "Relation has multiple outer ways: " << relation.id << '\n';

    relation.isComplete = complete;

    //std::cout << relation.data["tags"]["type"] << '\n';
}