#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <set>

namespace asc2 {

    class BuildingType {

        public:

            enum Type: uint32_t {
                UNKNOWN = 0,
                // ===== ACCOMODATION =====
                APPARTMENTS,
                BARRACKS,
                BUNGALOW,
                CABIN,
                DETACHED,
                DORMITORY,
                FARM,
                GER,
                HOTEL,
                HOUSE,
                HOUSEBOAT,
                RESIDENTIAL,
                SEMIDETACHED_HOUSE, // Doppelhaushälfte
                STATIC_CARAVAN,
                STILT_HOUSE,
                TERRACE,
                TREE_HOUSE,
                // ===== COMMERCIAL =====
                COMMERCIAL,
                INDUSTRIAL,
                KIOSK,
                OFFICE,
                RETAIL,
                SUPERMARKET,
                WAREHOUSE,
                // ===== RELIGIOS =====
                CATHEDRAL,
                CHAPEL,
                CHURCH,
                KINGDOM_HALL,
                MONASTARY,
                MOSQUE,
                PRESBYTERY,
                SHRINE,
                SYNAGOGUE,
                TEMPLE,
                RELIGIOUS_BUILDING,
                // ===== CIVIC / AMENITY =====
                BAKEHOUSE,
                BRIDGE,
                CIVIC,
                COLLEGE,
                FIRE_STATION,
                GOVERNMENT,
                GATEHOUSE,
                HOSPITAL,
                KINDERGARTEN,
                MUSEUM,
                PUBLIC,
                SCHOOL,
                TOILETS,
                TRAIN_STATION,
                TRANSPORTATION,
                UNIVERSITY,
                // ===== AGRICULTURE =====
                BARN,
                CONSERVATORY,
                COWSHED,
                FARM_AUXILIARY,
                GREENHOUSE,
                SLURY_TANK,
                STABLE,
                STY,
                LIVESTOCK,
                // ===== SPORTS =====
                GRANDSTAND,
                PAVILION,
                RIDING_HALL,
                SPORTS_HALL,
                STADIUM,
                // ===== STORAGE =====
                HANGAR,
                HUT,
                SHED,
                // ===== CARS =====
                CARPORT,
                GARAGE,
                GARAGES,
                PARKING,
                // ===== TECHNICAL BUILDINGS =====
                DIGESTER,
                SERVICE,
                TRANSFORMER_TOWER,
                WATER_TOWER,
                STORAGE_TANK,
                SILO,
                // ===== OTHER BUILDINGS =====
                BEACH_HUT,
                BUNKER,
                CASTLE,
                CONSTRUCTION,
                CONTAINER,
                MILITARY,
                ROOF,
                RUINS,
                TENT,
                TOWER,
                YES,
                // ===== USER DEFINED =====
                SPORTS_CENTRE,
                ALLOTMENT_HOUSE, // Strebergartenhaus
                CITY_GATE,
                GASOMETER,
                BELL_TOWER,
                SHELTER,
                FUNERAL_HALL,
                BOOTH,
                PYLON,
                BOATHOUSE,
                SWIMMING_HALL,
                STORAGE_BUILDING,
                ELECTRICITY,
                CLUBHOUSE,
                PUBLIC_BUILDING,
                BANK,
                COLLAPSED,
                BOAT,
                PREFABRICATED,
                RESTAURANT,
                MEMORIAL,
                HISTORIC,
                CARRIAGE,
                GATE,
                TRIUMPHAL_ARCH,
                SKYSCRAPER,
                SUBSTATION,
                SHIP,
                BEER_TENT,
                GUARDHOUSE,
                CINEMA,
                STAIRS,
                GARBAGE_SHED,
                MANUFACTURE,
                ELEVATOR,
                EXHIBITION_HALL,
                BICYCLE_PARKING,
                BREWERY,
                CHIMNEY,
                TOMB
            };

            static const std::set<BuildingType> ACCOMODATIONS;
            static const std::set<BuildingType> COMMERCIALS;
            static const std::set<BuildingType> RELIGIOUS;
            static const std::set<BuildingType> AMENITIES;
            static const std::set<BuildingType> AGRICULTURE;
            static const std::set<BuildingType> SPORTS;
            static const std::set<BuildingType> STORAGE;
            static const std::set<BuildingType> CARS;
            static const std::set<BuildingType> TECHNICAL;
            static const std::set<BuildingType> OTHER_BUILDINGS;
            static const std::set<BuildingType> USER_DEFINED;

            static const std::set<BuildingType>& getAll();

            BuildingType(const std::string& type);

            BuildingType(Type type) : type(type) {}

            bool operator==(Type type)         const { return type == this->type;      }
            bool operator==(BuildingType type) const { return this->type == type.type; }
            bool operator<(BuildingType type)  const { return this->type < type.type;  }

        private:

            Type type = Type::UNKNOWN;

            static const std::map<std::string, Type> typeNameMap;

            static std::set<BuildingType> allTypes;
    };
}