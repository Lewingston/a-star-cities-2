
#include "LineBuffer.h"
#include "../Map/Node.h"
#include "../Map/Way.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <numeric>

using namespace asc2;

LineBuffer::LineBuffer(const std::map<uint64_t, Way>& ways) {

    const std::size_t lineCount = std::accumulate(ways.begin(), ways.end(), 0u,
        [](std::size_t sum, const auto& way) {

            return sum + way.second.getLineCount();
        });

    if (!vertexBuffer.create(lineCount)) {
        throw std::runtime_error("Failed to create vertex buffer!");
    }

    vertexBuffer.setUsage(sf::VertexBuffer::Usage::Static);
    vertexBuffer.setPrimitiveType(sf::PrimitiveType::Lines);

    std::vector<sf::Vertex> vertexArray;
    vertexArray.reserve(lineCount);

    for (const auto& [id, way] : ways) {

        if (!way.isComplete)
            continue;

        addWayToVertexArray(way, vertexArray);
    }

    if (!vertexBuffer.update(vertexArray.data())) {
        throw std::runtime_error("Failed to initialize vertex buffer!");
    }
}

void LineBuffer::addWayToVertexArray(const Way& way,
                                     std::vector<sf::Vertex>& vertexArray) const {

    vertexArray.reserve(vertexArray.size() + way.getLineCount() * 2);

    for (auto iter = way.nodes.begin(); iter != way.nodes.end(); iter++) {

        const auto pos = sf::Vector2f{static_cast<float>(iter->get().x),
                                      static_cast<float>(iter->get().y)};

        const auto color = way.isComplete ? sf::Color::Black : sf::Color::Red;

        vertexArray.emplace_back(pos, color);

        // Add all nodes twic. Except first and last node in way
        if (iter != way.nodes.begin() && iter != way.nodes.end() - 1) {
            vertexArray.emplace_back(pos, color);
        }
    }
}

void LineBuffer::draw(sf::RenderTarget& target) {

    target.draw(vertexBuffer);
}