#pragma once

#include "Node.h"
#include "Way.h"
#include "Road.h"
#include "Building.h"
#include "Intersection.h"
#include "IdHandler.h"
#include "../MapUtilities/NetworkFinder.h"

#include <map>
#include <functional>

namespace asc2 {

    class Map {

        public:

            using ProjectionFunction = std::function<std::pair<double, double>(double, double)>;

            struct Dimensions {
                double minLon = 180.0f;
                double maxLon = -180.0f;
                double minLat = 90.0;
                double maxLat = -90.0;

                void adjust(double lon, double lat);
                std::pair<double, double> getCenter() const;
            };

            Map() = default;

            const Node& addNode(const Node& node);
            
            void addWay(const Way& way);

            void addRoad(RoadType type, uint64_t wayId);
            void addBuilding(BuildingType type, uint64_t wayId);
            void addBuilding(BuildingType type,
                             uint64_t relationId,
                             const std::vector<uint64_t>& outerWayIds,
                             const std::vector<uint64_t>& innerWayIds);

            void optimizeIntersecions();

            void splitRoadsOnIntersections();
            void removeUnnecessaryIntersections();
            void fuseRoads();
            void addIntersectionsToEndPoints();

            void segmentRoads(float length);
            std::vector<Road::NewRoadData> segmentRoad(Road& road, float length);

            std::size_t removeRoadsWithoutIntersections();
            void removeNetwork(const NetworkFinder::Network& network);

            [[nodiscard]] std::vector<std::reference_wrapper<const Node>> getNodes(
                const std::vector<uint64_t>& ids) const;
            [[nodiscard]] std::vector<std::reference_wrapper<const Way>> getWays(
                const std::vector<uint64_t>& ids) const;

            [[nodiscard]] const std::map<uint64_t, Node>&         getAllNodes()         const noexcept { return nodes;         }
            [[nodiscard]] const std::map<uint64_t, Way>&          getAllWays()          const noexcept { return ways;          }
            [[nodiscard]] const std::map<uint64_t, Intersection>& getAllIntersections() const noexcept { return intersections; }
            [[nodiscard]] const std::map<uint64_t, Road>&         getAllRoads()         const noexcept { return roads;         }

            [[nodiscard]] const std::vector<Building>& getAllBuildings() const noexcept { return buildings; }

            [[nodiscard]] std::pair<double, double> getCenter() const;

            [[nodiscard]] float getTotalRoadLength() const;

        private:

            void addRoad(RoadType type, const Way& way);
            void addRoadAndWay(const Road::NewRoadData& data);

            void addIntersectionsToEndOfRoad(Road& road);

            void removeIntersection(Intersection& intersection);
            void removeRoadAndWay(Road& road);

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;
            std::map<uint64_t, Intersection> intersections;
            std::map<uint64_t, Road> roads;
            std::vector<Building> buildings;

            Dimensions dimensions;

            IdHandler idHandler;

    };
}