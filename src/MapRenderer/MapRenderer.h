#pragma once

#include "RenderConfig.h"
#include "LineBuffer.h"
#include "ShapeBuffer.h"

#include <memory>

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

            std::unique_ptr<LineBuffer>  lineBufferAllWays;
            std::unique_ptr<LineBuffer>  lineBufferRoads;
            std::unique_ptr<LineBuffer>  lineBufferBuildings;
            std::unique_ptr<ShapeBuffer> shapeBufferBuildings;

            RenderConfig config;

    };
}