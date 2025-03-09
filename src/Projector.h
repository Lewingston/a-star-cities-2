#pragma once

namespace asc2 {

    class Map;

    class Projector {

        public:

            static void project(Map& map);

            static double distance(double angel, double radius);
            static double getRadAtLat(double lat);
    };
}