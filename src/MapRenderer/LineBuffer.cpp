
#include "LineBuffer.h"
#include "LineRenderer.h"
#include "Color.h"
#include "../Map/Node.h"
#include "../Map/Way.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <numeric>
#include <iostream>

using namespace asc2;

LineBuffer::LineBuffer(const std::vector<std::reference_wrapper<const Way>>& ways, const RenderConfig& config) :
    config(config) {

    const std::size_t lineCount = std::accumulate(ways.begin(), ways.end(), 0u,
        [](std::size_t sum, const auto& way) {
            return sum + way.get().getLineCount();
        });

    initVertexBuffer(lineCount * 2);

    for (const auto& way : ways) {

        if (!wayIsSuitableForRendering(way))
            continue;

        addWayToVertexArray(way, vertexArray, Color::getRandomColor());
    }

    updateVertexBuffer();
}

LineBuffer::LineBuffer(const std::map<uint64_t, Way>& ways, const RenderConfig& config) :
    config(config) {

    const std::size_t lineCount = std::accumulate(ways.begin(), ways.end(), 0u,
        [](std::size_t sum, const auto& way) {
            return sum + way.second.getLineCount();
        });

    initVertexBuffer(lineCount * 2);

    for (const auto& [id, way] : ways) {

        if (!wayIsSuitableForRendering(way))
            continue;

        addWayToVertexArray(way, vertexArray, sf::Color::Black);
    }

    updateVertexBuffer();
}

LineBuffer::LineBuffer(const std::vector<LineRenderer>& lines, const RenderConfig& config) :
    config(config) {

    const std::size_t lineCount = std::accumulate(lines.begin(), lines.end(), 0u,
        [](std::size_t sum, const LineRenderer& line) {
            return sum + line.getWay().getLineCount();
        });

    initVertexBuffer(lineCount * 2);

    for (const LineRenderer& line : lines) {

        addWayToVertexArray(line.getWay(), vertexArray, line.getColor());
    }

    updateVertexBuffer();
}

void LineBuffer::initVertexBuffer(std::size_t vertexCount) {

    if (!vertexBuffer.create(vertexCount)) {
        throw std::runtime_error("Failed to create vertex buffer!");
    }

    vertexBuffer.setUsage(sf::VertexBuffer::Usage::Static);
    vertexBuffer.setPrimitiveType(sf::PrimitiveType::Lines);

    vertexArray.clear();
    vertexArray.reserve(vertexCount);
}

void LineBuffer::updateVertexBuffer() {

    if (vertexArray.size() == 0) {
        std::cout << "Skip updating vertex buffer. Buffer has no vertices.\n";
        return;
    }

    if (!vertexBuffer.update(vertexArray.data(), vertexArray.size(), 0)) {
        throw std::runtime_error("Failed to udpate vertex buffer!");
    }
}

bool LineBuffer::wayIsSuitableForRendering(const Way& way) const {

    if (!config.displayIncomplete && !way.isComplete())
        return false;

    if (way.getNodeCount() <= 1)
        return false;

    return true;
}

void LineBuffer::addWayToVertexArray(const Way& way,
                                     std::vector<sf::Vertex>& vertexArray,
                                     sf::Color color) const {

    for (auto iter = way.getNodes().begin(); iter != way.getNodes().end(); iter++) {

        const auto pos = sf::Vector2f{static_cast<float>(iter->get().lon),
                                      -static_cast<float>(iter->get().lat)};

        //const auto color = way.isComplete ? sf::Color::Black : sf::Color::Red;

        vertexArray.emplace_back(pos, color);

        // Add all nodes twic. Except first and last node in way
        if (iter != way.getNodes().begin() && iter != way.getNodes().end() - 1) {
            vertexArray.emplace_back(pos, color);
        }
    }
}

void LineBuffer::draw(sf::RenderTarget& target) {

    target.draw(vertexBuffer);
}