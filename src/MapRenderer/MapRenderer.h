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

            [[nodiscard]] std::size_t getVertexCount()  const noexcept { return vertexCount;  }
            [[nodiscard]] std::size_t getEdgeCount()    const noexcept { return edgeCount;    }
            [[nodiscard]] std::size_t getPolygonCount() const noexcept { return polygonCount; }

        private:

            void createBufferFromAllWays(const Map& map);
            void createLineBufferFromRoads(const Map& map);
            void createLineBufferFromBuildings(const Map& map);
            void createShapeBufferFromBuildings(const Map& map);
            void createNodeBufferFromIntersections(const Map& map);
            void createBuffersForRoadNetworks(const Map& map);

            [[nodiscard]] std::size_t getBuildingWayCount(const Map& map) const;

            std::vector<std::unique_ptr<RenderBufferInterface>> renderBuffers;

            RenderConfig config;

            std::size_t vertexCount = 0;
            std::size_t edgeCount = 0;
            std::size_t polygonCount = 0;

    };
}