#pragma once

#include "Node.h"
#include "Way.h"
#include "Road.h"

#include <map>
#include <functional>

namespace asc2 {

    class Map {

        public:

            using ProjectionFunction = std::function<std::pair<double, double>(double, double)>;

            Map() = default;

            void addNode(const Node& node);
            
            void addWay(const Way& way);

            void addRoad(RoadType type, uint64_t wayId);

            void setCenter(std::pair<double, double> center) { this->center = center; }

            [[nodiscard]] std::vector<std::reference_wrapper<const Node>> getNodes(
                const std::vector<uint64_t>& nodeIds);

            [[nodiscard]] const std::map<uint64_t, Node>& getAllNodes() const noexcept;
            [[nodiscard]] const std::map<uint64_t, Way>&  getAllWays()  const noexcept;

            [[nodiscard]] const std::vector<Road>& getAllRoads() const noexcept { return roads; }

            [[nodiscard]] const std::pair<double, double>& getCenter() const noexcept { return center; }

        private:

            std::map<uint64_t, Node> nodes;
            std::map<uint64_t, Way>  ways;

            std::vector<Road> roads;

            std::pair<double, double> center;

    };
}