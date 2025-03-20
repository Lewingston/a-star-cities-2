
#include "Building.h"

using namespace asc2;

Building::Building(BuildingType type, const Way& way) :
    type(type),
    id (way.getId()),
    isRelation(false),
    outerWays({way}) {}

Building::Building(BuildingType type,
                   uint64_t buildingId,
                   const std::vector<std::reference_wrapper<const Way>>& outerWays,
                   const std::vector<std::reference_wrapper<const Way>>& innerWays) :
    type(type),
    id(buildingId),
    isRelation(true),
    outerWays(outerWays),
    innerWays(innerWays) {}