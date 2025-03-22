
#include "NetworkFinder.h"
#include "../Map/Map.h"
#include "../Map/Road.h"

#include <map>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace asc2;

NetworkFinder::NetworkFinder(Map& map) :
    map(map) {}

std::vector<NetworkFinder::Network> NetworkFinder::findNetworks(const Map& map) {

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

const std::vector<NetworkFinder::Network>& NetworkFinder::findNetworks() {

    networks = findNetworks(map);
    return networks;
}

NetworkFinder::Network NetworkFinder::findNetwork(const Intersection& intersection) {

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

void NetworkFinder::removeUnconnectedNetworks() {

    std::size_t removedRoadCount = map.removeRoadsWithoutIntersections();

    // find largest network
    auto largest = std::max_element(networks.begin(), networks.end(),
        [](const Network& n1, const Network& n2) {
            return n1.roads.size() < n2.roads.size();
        });

    for (auto network = networks.begin(); network != networks.end(); network++) {

        if (network == largest)
            continue;

        map.removeNetwork(*network);

        removedRoadCount += network->roads.size();
    }

    const std::size_t totalRoadCount = removedRoadCount + largest->roads.size();
    const float percentage = static_cast<float>(removedRoadCount) / static_cast<float>(totalRoadCount) * 100.0f;

    std::cout << "Removed " << removedRoadCount << " of " << totalRoadCount << " roads (" << percentage << "%)\n";
}