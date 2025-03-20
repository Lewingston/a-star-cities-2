
#include "ShapeRenderer.h"
#include "../Map/Node.h"

#include <mapbox/earcut.hpp>

#include <array>
#include <utility>

using namespace asc2;

ShapeRenderer::ShapeRenderer(const std::vector<std::reference_wrapper<const Way>>& outerShapes,
                             const std::vector<std::reference_wrapper<const Way>>& innerShapes) :
    outerShapes(outerShapes),
    innerShapes(innerShapes) {}

std::vector<std::pair<float, float>> ShapeRenderer::getVertices() const {

    std::vector<std::pair<float, float>> vertices;

    for (const Way& shape : outerShapes) {

        const auto ver = getVerticesForShape(shape);

        vertices.insert(vertices.end(), ver.begin(), ver.end());
    }

    return vertices;
}

std::vector<std::pair<float, float>> ShapeRenderer::getVerticesForShape(const Way& outerShape) const {

    std::vector<std::vector<std::array<double, 2>>> points;
    points.reserve(innerShapes.size() + 1);

    // first add outer shape
    points.push_back({});
    points.back().reserve(outerShape.getNodeCount() - 1);
    for (auto iter = outerShape.getNodes().begin(); iter != outerShape.getNodes().end() - 1; iter++) {
        points.back().push_back({iter->get().lon, iter->get().lat});
    }

    // add inner shapes
    for (const Way& innerWay : innerShapes) {
        points.push_back({});
        points.back().reserve(innerWay.getNodeCount() - 1);
        for (auto iter = innerWay.getNodes().begin(); iter != innerWay.getNodes().end() - 1; iter++) {
            points.back().push_back({iter->get().lon, iter->get().lat});
        }
    }

    // run tesselation
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(points);

    std::vector<std::pair<float, float>> vertices;
    vertices.reserve(indices.size());

    for (uint32_t index : indices) {

        std::size_t indexX = 0;
        std::size_t indexY = index;

        while (indexY >= points.at(indexX).size()) {
            indexY -= points.at(indexX).size();
            indexX++;
        }

        const auto [lon, lat] = points.at(indexX).at(indexY);

        vertices.emplace_back(
            static_cast<float>(lon),
            static_cast<float>(lat)
        );
    }

    return vertices;
}