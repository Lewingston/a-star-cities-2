
#include "BuildingType.h"

using namespace asc2;

const std::map<std::string, BuildingType::Type> BuildingType::typeNameMap = {
    // ===== ACCOMODATION =====
    { "apartments",         APPARTMENTS        },
    { "barracks",           BARRACKS           },
    { "bungalow",           BUNGALOW           },
    { "cabin",              CABIN              },
    { "detached",           DETACHED           },
    { "dormitory",          DORMITORY          },
    { "farm",               FARM               },
    { "ger",                GER                },
    { "hotel",              HOTEL              },
    { "house",              HOUSE              },
    { "houseboat",          HOUSEBOAT          },
    { "residential",        RESIDENTIAL        },
    { "semidetached_house", SEMIDETACHED_HOUSE },
    { "static_caravan",     STATIC_CARAVAN     },
    { "still_house",        STILT_HOUSE        },
    { "terrace",            TERRACE            },
    { "tree_house",         TREE_HOUSE         },
    // ===== COMMERCIAL =====
    { "commercial",         COMMERCIAL         },
    { "industrial",         INDUSTRIAL         },
    { "kiosk",              KIOSK              },
    { "office",             OFFICE             },
    { "retail",             RETAIL             },
    { "supermarket",        SUPERMARKET        },
    { "warehouse",          WAREHOUSE          },
    // ===== RELIGIOS =====
    { "cathedral",          CATHEDRAL          },
    { "chapel",             CHAPEL             },
    { "church",             CHURCH             },
    { "kingdom_hall",       KINGDOM_HALL       },
    { "monastary",          MONASTARY          },
    { "monastery",          MONASTARY          },
    { "mosque",             MOSQUE             },
    { "presbytery",         PRESBYTERY         },
    { "shrine",             SHRINE             },
    { "synagogue",          SYNAGOGUE          },
    { "temple",             TEMPLE             },
    { "religious",          RELIGIOUS_BUILDING },
    // ===== CIVIC / AMENITY =====
    { "bakehouse",          BAKEHOUSE          },
    { "bridge",             BRIDGE             },
    { "civic",              CIVIC              },
    { "college",            COLLEGE            },
    { "fire_station",       FIRE_STATION       },
    { "government",         GOVERNMENT         },
    { "gatehouse",          GATEHOUSE          },
    { "hospital",           HOSPITAL           },
    { "kindergarten",       KINDERGARTEN       },
    { "museum",             MUSEUM             },
    { "public",             PUBLIC             },
    { "school",             SCHOOL             },
    { "toilets",            TOILETS            },
    { "train_station",      TRAIN_STATION      },
    { "transportation",     TRANSPORTATION     },
    { "university",         UNIVERSITY         },
    // ===== AGRICULTURE =====
    { "barn",               BARN               },
    { "conservatory",       CONSERVATORY       },
    { "cowsehd",            COWSHED            },
    { "farm_auxiliary",     FARM_AUXILIARY     },
    { "greenhouse",         GREENHOUSE         },
    { "slury_tank",         SLURY_TANK         },
    { "stable",             STABLE             },
    { "sty",                STY,               },
    { "livestock",          LIVESTOCK          },
    // ===== SPORTS =====
    { "grandstand",         GRANDSTAND         },
    { "pavilion",           PAVILION           },
    { "riding_hall",        RIDING_HALL        },
    { "sports_hall",        SPORTS_HALL        },
    { "stadium",            STADIUM            },
    // ===== STORAGE =====
    { "hangar",             HANGAR             },
    { "hut",                HUT                },
    { "shed",               SHED               },
    // ===== CARS =====
    { "carport",            CARPORT            },
    { "garage",             GARAGE             },
    { "garages",            GARAGES            },
    { "parking",            PARKING            },
    // ===== TECHNICAL BUILDINGS =====
    { "digester",           DIGESTER,          },
    { "service",            SERVICE,           },
    { "transformer_tower",  TRANSFORMER_TOWER  },
    { "water_tower",        WATER_TOWER        },
    { "storage_tank",       STORAGE_TANK       },
    { "silo",               SILO               },
    // ===== OTHER_BUILDINGS =====
    { "beach_hut",          BEACH_HUT          },
    { "bunker",             BUNKER             },
    { "castle",             CASTLE             },
    { "construction",       CONSTRUCTION       },
    { "container",          CONTAINER          },
    { "military",           MILITARY           },
    { "roof",               ROOF               },
    { "ruins",              RUINS              },
    { "tent",               TENT               },
    { "tower",              TOWER              },
    { "yes",                YES                },
    // ===== USER DEFINED =====
    { "sports_centre",      SPORTS_CENTRE      },
    { "allotment_house",    ALLOTMENT_HOUSE    },
    { "city_gate",          CITY_GATE          },
    { "gasometer",          GASOMETER          },
    { "bell_tower",         BELL_TOWER         },
    { "shelter",            SHELTER            },
    { "funeral_hall",       FUNERAL_HALL       },
    { "booth",              BOOTH              },
    { "pylon",              PYLON              },
    { "boathouse",          BOATHOUSE          },
    { "swimming_hall",      SWIMMING_HALL      },
    { "storage",            STORAGE_BUILDING   },
    { "electricity",        ELECTRICITY        },
    { "clubhouse",          CLUBHOUSE          },
    { "public_building",    PUBLIC_BUILDING    },
    { "bank",               BANK               },
    { "collapsed",          COLLAPSED          },
    { "boat",               BOAT               },
    { "prefabricated",      PREFABRICATED      },
    { "restaurant",         RESTAURANT         },
    { "memorial",           MEMORIAL           },
    { "historic",           HISTORIC           },
    { "carriage",           CARRIAGE           },
    { "gate",               GATE               },
    { "triumphal_arch",     TRIUMPHAL_ARCH     },
    { "skyscraper",         SKYSCRAPER         },
    { "substation",         SUBSTATION         },
    { "ship",               SHIP               },
    { "beer_tent",          BEER_TENT          },
    { "guardhouse",         GUARDHOUSE         },
    { "cinema",             CINEMA             },
    { "stairs",             STAIRS             },
    { "garbage_shed",       GARBAGE_SHED       },
    { "manufacture",        MANUFACTURE        },
    { "elevator",           ELEVATOR           },
    { "exhibition_hall",    EXHIBITION_HALL    },
    { "bicycle_parking",    BICYCLE_PARKING    },
    { "brewery",            BREWERY            },
    { "chimney",            CHIMNEY            },
    { "tomb",               TOMB               }
};

const std::set<BuildingType> BuildingType::ACCOMODATIONS = {
    APPARTMENTS, BARRACKS,    BUNGALOW, CABIN,    DETACHED,
    DORMITORY,   FARM,        GER,      HOTEL,    HOUSE,
    HOUSEBOAT,   RESIDENTIAL, SEMIDETACHED_HOUSE, STATIC_CARAVAN,
    STILT_HOUSE
};

const std::set<BuildingType> BuildingType::COMMERCIALS = {
    COMMERCIAL, INDUSTRIAL, KIOSK, OFFICE, RETAIL, SUPERMARKET, WAREHOUSE
};

const std::set<BuildingType> BuildingType::RELIGIOUS = {
    CATHEDRAL,  CHAPEL, CHURCH,    KINGDOM_HALL, MONASTARY, MOSQUE,
    PRESBYTERY, SHRINE, SYNAGOGUE, TEMPLE,       RELIGIOUS_BUILDING
};

const std::set<BuildingType> BuildingType::AMENITIES = {
    BAKEHOUSE, BRIDGE,        CIVIC,          COLLEGE, FIRE_STATION, GOVERNMENT,
    GATEHOUSE, HOSPITAL,      KINDERGARTEN,   MUSEUM,  PUBLIC,       SCHOOL,
    TOILETS,   TRAIN_STATION, TRANSPORTATION, UNIVERSITY
};

const std::set<BuildingType> BuildingType::AGRICULTURE = {
    BARN,   CONSERVATORY, COWSHED, FARM_AUXILIARY, GREENHOUSE, SLURY_TANK,
    STABLE, STY,          LIVESTOCK
};

const std::set<BuildingType> BuildingType::SPORTS = {
    GRANDSTAND, PAVILION, RIDING_HALL, SPORTS_HALL, STADIUM
};

const std::set<BuildingType> BuildingType::STORAGE = {
    HANGAR, HUT, SHED
};

const std::set<BuildingType> BuildingType::CARS = {
    CARPORT, GARAGE, GARAGES, PARKING
};

const std::set<BuildingType> BuildingType::TECHNICAL = {
    DIGESTER, SERVICE, TRANSFORMER_TOWER, WATER_TOWER, STORAGE_TANK, SILO
};

const std::set<BuildingType> BuildingType::OTHER_BUILDINGS = {
    BEACH_HUT, BUNKER, CASTLE, CONSTRUCTION, CONTAINER, MILITARY,
    ROOF,      RUINS,  TENT,   TOWER,        YES
};

const std::set<BuildingType> BuildingType::USER_DEFINED = {
    SPORTS_CENTRE,   ALLOTMENT_HOUSE,  CITY_GATE,    GASOMETER,     BELL_TOWER,
    SHELTER,         FUNERAL_HALL,     BOOTH,        PYLON,         BOATHOUSE,
    SWIMMING_HALL,   STORAGE_BUILDING, ELECTRICITY,  CLUBHOUSE,     PUBLIC_BUILDING,
    BANK,            COLLAPSED,        BOAT,         PREFABRICATED, RESTAURANT,
    MEMORIAL,        HISTORIC,         CARRIAGE,     GATE,          TRIUMPHAL_ARCH,
    SKYSCRAPER,      SUBSTATION,       SHIP,         BEER_TENT,     GUARDHOUSE,
    CINEMA,          STAIRS,           GARBAGE_SHED, MANUFACTURE,   ELEVATOR,
    EXHIBITION_HALL, BICYCLE_PARKING,  BREWERY,      CHIMNEY,       TOMB
};

std::set<BuildingType> BuildingType::allTypes;

const std::set<BuildingType>& BuildingType::getAll() {
    if (allTypes.size() == 0) {
        allTypes.insert(ACCOMODATIONS.begin(), ACCOMODATIONS.end());
        allTypes.insert(COMMERCIALS.begin(), COMMERCIALS.end());
        allTypes.insert(RELIGIOUS.begin(), RELIGIOUS.end());
        allTypes.insert(AMENITIES.begin(), AMENITIES.end());
        allTypes.insert(AGRICULTURE.begin(), AGRICULTURE.end());
        allTypes.insert(SPORTS.begin(), SPORTS.end());
        allTypes.insert(STORAGE.begin(), STORAGE.end());
        allTypes.insert(CARS.begin(), CARS.end());
        allTypes.insert(TECHNICAL.begin(), TECHNICAL.end());
        allTypes.insert(OTHER_BUILDINGS.begin(), OTHER_BUILDINGS.end());
        allTypes.insert(USER_DEFINED.begin(), USER_DEFINED.end());
    }
    return allTypes;
}

BuildingType::BuildingType(const std::string& type) {
    if (auto find = typeNameMap.find(type); find != typeNameMap.end()) {
        this->type = find->second;
    } else {
        this->type = UNKNOWN;
    }
}