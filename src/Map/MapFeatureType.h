
#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <set>
#include <map>

namespace asc2 {

    class MapFeatureType {

        public:

            enum Type : uint16_t {
                NO_TYPE = 0,
                AERIALWAY,
                AEROWAY,
                AMENITY,
                ATTRACTION,
                BARRIER,
                BOUNDARY,
                BUILDING,
                CRAFT,
                EMERGENCY,
                GEOLOGICAL,
                HEALTHCARE,
                HIGHWAY,
                HISTORIC,
                LANDUSE,
                LEISURE,
                MAN_MADE,
                MILITARY,
                NATURAL,
                OFFICE,
                PLACE,
                POWER,
                PUBLIC_TRANSPORT,
                RAILWAY,
                ROUTE,
                SHOP,
                SPORT,
                TELECOM,
                TOURISM,
                WATER,
                WATERWAY,
                BUILDING_PART,
                UNKNOWN = std::numeric_limits<uint16_t>::max()
            };

            [[nodiscard]] static const std::set<MapFeatureType>& getAll();

            [[nodiscard]] static const std::string& getTypeName(MapFeatureType type);
            [[nodiscard]] static const std::string& getTypeName(Type type);

            MapFeatureType(Type type) : type(type) {}

            bool operator==(Type type)           const { return type == this->type; }
            bool operator==(MapFeatureType type) const { return this->type == type.type; }
            bool operator<(MapFeatureType type)  const { return this->type < type.type; }

        private:

            Type type = Type::UNKNOWN;

            static const std::map<std::string, Type> nameTypeMap;
            static const std::map<Type, std::string> typeNameMap;

            static std::set<MapFeatureType> allTypes;
    };
}