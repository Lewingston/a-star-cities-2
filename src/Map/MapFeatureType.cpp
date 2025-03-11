
#include "MapFeatureType.h"

using namespace asc2;

const std::map<std::string, MapFeatureType::Type> MapFeatureType::nameTypeMap = {
    { "aerialway",        Type::AERIALWAY        },
    { "aeroway",          Type::AEROWAY          },
    { "amenity",          Type::AMENITY          },
    { "attraction",       Type::ATTRACTION       },
    { "barrier",          Type::BARRIER          },
    { "boundary",         Type::BOUNDARY         },
    { "building",         Type::BUILDING         },
    { "craft",            Type::CRAFT            },
    { "emergency",        Type::EMERGENCY        },
    { "geological",       Type::GEOLOGICAL       },
    { "healthcare",       Type::HEALTHCARE       },
    { "highway",          Type::HIGHWAY          },
    { "historic",         Type::HISTORIC         },
    { "landuse",          Type::LANDUSE          },
    { "leisure",          Type::LEISURE          },
    { "man_made",         Type::MAN_MADE         },
    { "military",         Type::MILITARY         },
    { "natural",          Type::NATURAL          },
    { "office",           Type::OFFICE           },
    { "place",            Type::PLACE            },
    { "power",            Type::POWER            },
    { "public_transport", Type::PUBLIC_TRANSPORT },
    { "railway",          Type::RAILWAY          },
    { "route",            Type::ROUTE            },
    { "shop",             Type::SHOP             },
    { "sport",            Type::SPORT            },
    { "telecom",          Type::TELECOM          },
    { "tourism",          Type::TOURISM          },
    { "water",            Type::WATER            },
    { "waterway",         Type::WATERWAY         },
    { "building:part",    Type::BUILDING_PART    },
};

const std::map<MapFeatureType::Type, std::string> MapFeatureType::typeNameMap = []() {

    std::map<MapFeatureType::Type, std::string> map;

    for (const auto& [name, type] : MapFeatureType::nameTypeMap) {
        map.emplace(type, name);
    }

    return map;
}();

std::set<MapFeatureType> MapFeatureType::allTypes = {
    Type::AERIALWAY
};

const std::set<MapFeatureType>& MapFeatureType::getAll() {
    return allTypes;
}

const std::string& MapFeatureType::getTypeName(MapFeatureType type) {

    return getTypeName(type.type);
}

const std::string& MapFeatureType::getTypeName(MapFeatureType::Type type) {

    static const std::string unknownTypeName = "UNKNOWN";

    if (auto find = typeNameMap.find(type); find != typeNameMap.end()) {
        return find->second;
    } else {
        return unknownTypeName;
    }
}