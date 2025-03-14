#pragma once

#include "Node.h"
#include "Way.h"
#include "Road.h"
#include "Building.h"

#include <map>
#include <functional>

namespace asc2 {

    class Map {

        public:

            using ProjectionFunction = std::function<std::pair<double, double>(double, double)>;

            struct Dimensions {
                double minLon = 180.0f;
                double maxLon = 0.0f;
                double minLat = 90.0;
                double maxLat = 0.0;

                void adjust(double lon, double lat);
                std::pair<double, double> getCenter() const;
            };

            Map() = default;

            void addNode(const Node& node);
            
            void addWay(const Way& way);

            void addRoad(RoadType type, uint64_t wayId);
            void addBuilding(BuildingType type, uint64_t wayId);
            void addBuilding(BuildingType type,
                             uint64_t relationId,
                             const std::vector<uint64_t>& outerWayIds,
                             const std::vector<uint64_t>& innerWayIds);

            [[nodiscard]] std::vector<std::reference_wrapper<const Node>> getNodes(
                const std::vector<uint64_t>& ids) const;
            [[nodiscard]] std::vector<std::reference_wrapper<const Way>> getWays(
                const std::vector<uint64_t>& ids) const;

            [[nodiscard]] const std::map<uint64_t, Node>& getAllNodes() const noexcept { return nodes; }
            [[nodiscard]] const std::map<uint64_t, Way>&  getAllWays()  const noexcept { return ways;  }

            [[nodiscard]] const std::vector<Road>&     getAllRoads()     const noexcept { return roads;     }
            [[nodiscard]] const std::vector<Building>& getAllBuildings() const noexcept { return buildings; }

            [[nodiscard]] std::pair<double, double> getCenter() const;

        private:

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;

            std::vector<Road> roads;
            std::vector<Building> buildings;

            Dimensions dimensions;

    };
}