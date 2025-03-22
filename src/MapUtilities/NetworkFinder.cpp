
#include "NetworkFinder.h"
#include "../Map/Map.h"

#include <map>
#include <stack>
#include <iostream>

using namespace asc2;

NetworkFinder::NetworkFinder(const Map& map) :
    map(map) {}

std::vector<NetworkFinder::Network> NetworkFinder::findNetworks() const {

    std::vector<Network> networks;

    std::map<uint64_t, std::reference_wrapper<const Intersection>> allIntersections;
    for (const auto& [id, intersection] : map.getAllIntersections()) {
        allIntersections.emplace(id, intersection);
    }

    while (allIntersections.size() > 0) {

        const Intersection& inter = allIntersections.begin()->second;
        networks.emplace_back(findNetwork(inter));

        for (const auto& [id, intersection] : networks.back().intersections) {
            allIntersections.erase(id);
        }
    }

    std::cout << "Number of networks: " << networks.size() << '\n';

    return networks;
}

NetworkFinder::Network NetworkFinder::findNetwork(const Intersection& intersection) const {

    Network network;

    std::stack<std::reference_wrapper<const Intersection>> intersectionStack;
    intersectionStack.push(intersection);

    while (!intersectionStack.empty()) {

        const Intersection& intersection = intersectionStack.top();
        intersectionStack.pop();

        const auto connections = intersection.getConnections(false);
        for (const auto& connection : intersection.getConnections(false)) {
            if (!network.intersections.contains(connection.intersection.getId()))
                intersectionStack.push(connection.intersection);
            network.roads.emplace(connection.road.getId(), connection.road);
        }

        network.intersections.emplace(intersection.getId(), intersection);
    }

    return network;
}