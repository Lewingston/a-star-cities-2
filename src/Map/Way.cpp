
#include "Way.h"
#include "Intersection.h"

#include <iostream>

using namespace asc2;

Way::Way(uint64_t id, const std::vector<std::reference_wrapper<const Node>>& nodes, bool isComplete) :
    id(id),
    nodes(nodes),
    complete(isComplete) {}

bool Way::isLoop() const {

    return nodes.front().get().id == nodes.back().get().id;
}

float Way::getLength() const {

    float length = 0.0f;

    for (auto iter = nodes.begin(); iter != nodes.end() - 1; ++iter) {

        const Node& node = iter->get();
        const Node& nextNode = std::next(iter)->get();

        length += node.distance(nextNode);
    }

    return length;
}

std::vector<Way::NewNode> Way::segmentate(float length) const {

    std::vector<NewNode> newNodes;

    const float wayLength = getLength();
    const uint32_t totalSegments = static_cast<uint32_t>(wayLength / length);

    if (totalSegments <= 1)
        return newNodes;

    const float segmentLength = wayLength / static_cast<float>(totalSegments);

    float currentSegmentLength = 0.0f;

    std::uint32_t index = 1;

    for (auto nodeIter = nodes.begin(); nodeIter != nodes.end() - 1; nodeIter++) {

        const Node& currentNode = nodeIter->get();
        const Node& nextNode    = *std::next(nodeIter);

        const float nodeDistance = currentNode.distance(nextNode);
        float distanceToNextNode = nodeDistance;

        while (currentSegmentLength + distanceToNextNode > segmentLength) {

            const float length = (nodeDistance - distanceToNextNode) + (segmentLength - currentSegmentLength);
            const auto [x, y] = currentNode.pointBetween(nextNode, length);
            newNodes.emplace_back(x, y, index);
            index++;

            if (newNodes.size() + 1 >= totalSegments) {
                return newNodes;
            }

            distanceToNextNode -= (segmentLength - currentSegmentLength);
            currentSegmentLength = 0;
        }

        currentSegmentLength += distanceToNextNode;
        index++;
    }

    if (totalSegments != newNodes.size() + 1) {
        std::cerr << "Incorrect number of segments: " << totalSegments << " vs. " << (newNodes.size() + 1) << '\n';
    }

    return newNodes;
}