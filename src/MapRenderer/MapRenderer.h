#pragma once

#include "RenderConfig.h"
#include "RenderBufferInterface.h"

#include <memory>
#include <vector>

namespace sf {
    class RenderTarget;
}

namespace asc2 {

    class Map;

    class MapRenderer {

        public:

            MapRenderer() = default;

            void init(const Map& map,
                      const RenderConfig& config);

            void draw(sf::RenderTarget& target);

        private:

            void createBufferFromAllWays(const Map& map);
            void createLineBufferFromRoads(const Map& map);
            void createLineBufferFromBuildings(const Map& map);
            void createShapeBufferFromBuildings(const Map& map);
            void createNodeBufferFromIntersections(const Map& map);

            [[nodiscard]] std::size_t getBuildingWayCount(const Map& map) const;

            std::vector<std::unique_ptr<RenderBufferInterface>> renderBuffers;

            RenderConfig config;

    };
}