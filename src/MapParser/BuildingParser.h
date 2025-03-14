#pragma once

#include "WayParser.h"
#include "RelationParser.h"

#include "../Map/BuildingType.h"

namespace asc2 {

    class BuildingParser {

        public:

            using json = nlohmann::json;

            [[nodiscard]] static BuildingType getBuildingType(const WayParser::Way& way);
            [[nodiscard]] static BuildingType getBuildingType(const RelationParser::Relation& relation);
            [[nodiscard]] static BuildingType getBuildingType(const json& data);

        private:

    };
}