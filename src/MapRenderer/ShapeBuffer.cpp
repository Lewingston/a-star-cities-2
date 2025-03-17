
#include "ShapeBuffer.h"
#include "ShapeRenderer.h"

#include <SFML/Graphics/RenderTArget.hpp>

#include <iostream>
#include <numeric>
#include <string>

using namespace asc2;

ShapeBuffer::ShapeBuffer(const std::vector<ShapeRenderer>& shapes, bool lineMode) :
    lineMode(lineMode) {

    const sf::Color color = sf::Color::Black;

    for (const ShapeRenderer& shape : shapes) {

        const auto vertices = shape.getVertices();
        if (vertices.size() % 3 != 0)
            throw std::runtime_error("Shape has invalid vertex count: " + std::to_string(vertices.size()));

        for (std::size_t ii = 0; ii < vertices.size(); ii += 3) {

            if (lineMode) {
                vertexArray.emplace_back(sf::Vector2f{vertices[ii].first, -vertices[ii].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 1].first, -vertices[ii + 1].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 1].first, -vertices[ii + 1].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 2].first, -vertices[ii + 2].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 2].first, -vertices[ii + 2].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii].first, -vertices[ii].second}, color);
            } else {
                vertexArray.emplace_back(sf::Vector2f{vertices[ii].first, -vertices[ii].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 1].first, -vertices[ii + 1].second}, color);
                vertexArray.emplace_back(sf::Vector2f{vertices[ii + 2].first, -vertices[ii + 2].second}, color);
            }
        }
    }

    initVertexBuffer(vertexArray.size());

    updateVertexBuffer();
}

void ShapeBuffer::initVertexBuffer(std::size_t vertexCount) {

    if (!vertexBuffer.create(vertexCount)) {
        throw std::runtime_error("Failed to create vertex buffer!");
    }

    vertexBuffer.setUsage(sf::VertexBuffer::Usage::Static);
    vertexBuffer.setPrimitiveType(lineMode ? sf::PrimitiveType::Lines : sf::PrimitiveType::Triangles);
}

void ShapeBuffer::updateVertexBuffer() {

    if (vertexArray.size() == 0) {
        std::cout << "Skip updating vertex buffer. Buffer has no vertices.\n";
        return;
    }

    if (!vertexBuffer.update(vertexArray.data(), vertexArray.size(), 0)) {
        throw std::runtime_error("Filed to update vertex buffer!");
    }
}

void ShapeBuffer::draw(sf::RenderTarget& target) {

    target.draw(vertexBuffer);
}

std::size_t ShapeBuffer::getVertexCount() const noexcept {

    return vertexArray.size();
}

std::size_t ShapeBuffer::getEdgeCount() const noexcept {

    if (lineMode)
        return getVertexCount() / 2;
    else
        return 0;
}

std::size_t ShapeBuffer::getPolygonCount() const noexcept {

    if (lineMode)
        return 0;
    else
        return getVertexCount() / 3;
}