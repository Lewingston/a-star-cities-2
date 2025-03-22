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

            NetworkFinder(Map& map);

            [[nodiscard]] static std::vector<Network> findNetworks(const Map& map);

            const std::vector<Network>& findNetworks();

            [[nodiscard]] static Network findNetwork(const Intersection& intersection);

            void removeUnconnectedNetworks();

        private:

            Map& map;

            std::vector<Network> networks;

    };
}