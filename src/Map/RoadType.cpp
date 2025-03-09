
#include "RoadType.h"

#include <iostream>

using namespace asc2;

const std::map<std::string, RoadType::Type> RoadType::typeNameMap = {
    // ===== ROADS =====
    { "motorway",       MOTORWAY       },
    { "trunk",          TRUNK          },
    { "primary",        PRIMARY        },
    { "secondary",      SECONDARY      },
    { "tertiary",       TERTIARY       },
    { "unclassified",   UNCLASSIFIED   },
    { "residential",    RESIDENTIAL    },
    // ===== LINK ROADS =====
    { "motorway_link",  MOTORWAY_LINK  },
    { "trunk_link",     TRUNK_LINK     },
    { "primary_link",   PRIMARY_LINK   },
    { "secondary_link", SECONDARY_LINK },
    { "tertiary_link",  TERTIARY_LINK  },
    // ===== SPECIAL ROAD TYPES =====
    { "living_street",  LIVING_STREET  },
    { "service",        SERVICE        },
    { "pedestrian",     PEDESTRIAN     },
    { "track",          TRACK          },
    { "bus_guideway",   BUS_GUIDEWAY   },
    { "escape",         ESCAPE         },
    { "road",           ROAD           },
    { "busway",         BUSWAY         },
    // ===== PATHS =====
    { "footway",        FOOTWAY        },
    { "bridleway",      BRIDLEWAY      },
    { "steps",          STEPS          },
    { "corridor",       CORRIDOR       },
    { "path",           PATH           },
    { "via_ferrata",    VIA_FERRATA    },
    // ===== CYCLEWAY =====
    { "cycleway",       CYCLEWAY       },
    // ===== LIFECYCLE =====
    { "construction",   CONSTRUCTION   },
    // ===== OTHER HIGHWAY FEATURES =====
    { "elevator",       ELEVATOR       },
    { "emergency_bay",  EMERGENCY_BAY  },
    { "platform",       PLATFORM       },
    { "rest_area",      REST_AREA      },
    { "services",       SERVICES       },
    { "bus_stop",       BUS_STOP       },
    { "crossing",       CROSSING       },
    { "proposed",       PROPOSED       },
    { "razed",          RAZED          }, 
};

const std::set<RoadType> RoadType::ROADS = {
    MOTORWAY, TRUNK, PRIMARY, SECONDARY, TERTIARY, UNCLASSIFIED, RESIDENTIAL
};

const std::set<RoadType> RoadType::LINKS = {
    MOTORWAY_LINK, TRUNK_LINK, PRIMARY_LINK, SECONDARY_LINK, TERTIARY_LINK
};

const std::set<RoadType> RoadType::SPECIAL = {
    LIVING_STREET, SERVICE, PEDESTRIAN, TRACK, BUS_GUIDEWAY, ESCAPE, ROAD, BUSWAY
};

const std::set<RoadType> RoadType::PATHS = {
    FOOTWAY, BRIDLEWAY, STEPS, CORRIDOR, PATH, VIA_FERRATA
};

const std::set<RoadType> RoadType::OTHERS = {
    CYCLEWAY,  CONSTRUCTION, PROPOSED, ELEVATOR, EMERGENCY_BAY, PLATFORM,
    REST_AREA, SERVICES,     BUS_STOP, CROSSING, RAZED
};

std::set<RoadType> RoadType::allTypes;

const std::set<RoadType>& RoadType::getAll() {
    if (allTypes.size() == 0) {
        allTypes.insert(ROADS.begin(),    ROADS.end());
        allTypes.insert(LINKS.begin(),    LINKS.end());
        allTypes.insert(SPECIAL.begin(),  SPECIAL.end());
        allTypes.insert(PATHS.begin(),    PATHS.end());
        allTypes.insert(OTHERS.begin(),   OTHERS.end());
    }
    return allTypes;
}

RoadType::RoadType(const std::string& type) {

    if (auto find = typeNameMap.find(type); find != typeNameMap.end()) {
        this->type = find->second;
    } else {
        std::cout << "Unknown road type: " << type << '\n';
        this->type = UNKNOWN;
    }
}