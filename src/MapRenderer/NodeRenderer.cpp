
#include "NodeRenderer.h"

#include <numbers>
#include <cmath>

using namespace asc2;

float NodeRenderer::size = 360.0f / 40'000'000.0f * 5.0f;
uint8_t NodeRenderer::vertexCount = 12;

std::vector<std::pair<float, float>> NodeRenderer::vertices = createVertices(size, vertexCount);

NodeRenderer::NodeRenderer(float posX, float posY) :
    posX(posX),
    posY(posY) {}

void NodeRenderer::setGeometry(float size, uint8_t vertexCount) {

    NodeRenderer::vertexCount = vertexCount;
    NodeRenderer::size = 360.0f / 40'000'000.0f * size;

    vertices = createVertices(NodeRenderer::size, NodeRenderer::vertexCount);
}

const std::vector<std::pair<float, float>>& NodeRenderer::getVertices() const {

    const float xOffset = vertices[0].first;
    const float yOffset = vertices[0].second;

    for (std::pair<float, float>& pos : vertices) {
        pos.first  = pos.first  - xOffset + posX;
        pos.second = pos.second - yOffset + posY;
    }

    return vertices;
}

std::vector<std::pair<float, float>> NodeRenderer::createVertices(float size, uint8_t vertexCount) {

    std::vector<std::pair<float, float>> vertices(vertexCount * 3);

    const auto calcVertexPos = [](float angel, float radius) -> std::pair<float, float> {
        const float posX = std::cos(angel) * radius;
        const float posY = std::sin(angel) * radius;
        return { posX, posY };
    };

    for (uint16_t ii = 0; ii < vertexCount; ii++) {

        // center node
        vertices[ii * 3] = { 0.0f, 0.0f };

        // left
        {
        const float angel = std::numbers::pi_v<float> * 2 / vertexCount * ii;
        vertices[ii * 3u + 1u] = calcVertexPos(angel, size);
        }

        // right
        {
        const float angel = std::numbers::pi_v<float> * 2 / vertexCount * (ii + 1);
        vertices[ii * 3u + 2u] = calcVertexPos(angel, size);
        }
    }

    return vertices;
}
