#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace asc2 {

    class Map;
    class Road;
    class Intersection;

    class NetworkFinder {

        public:

            struct Network {
                std::map<uint64_t, std::reference_wrapper<const Intersection>> intersections;
                std::map<uint64_t, std::reference_wrapper<const Road>> roads;
            };

            NetworkFinder(const Map& map);

            std::vector<Network> findNetworks() const;

            Network findNetwork(const Intersection& intersection) const;

        private:

            const Map& map;

    };
}