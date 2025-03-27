
#include "AStarOverlay.h"

#include <iostream>

using namespace asc2;

AStarOverlay::AStarOverlay(const sf::Vector2u& size) :
    sprite(textures[0].getTexture()) {

    setResolution(size);
}

void AStarOverlay::setResolution(sf::Vector2u resolution) {

    setResolution(textures[0], resolution);
    setResolution(textures[1], resolution);
}

void AStarOverlay::setSize(sf::Vector2f size) {

    this->size = size;

    const sf::Vector2f res = sf::Vector2f(
        static_cast<float>(textures[0].getSize().x),
        static_cast<float>(textures[0].getSize().y)
    );

    const sf::Vector2f scale = sf::Vector2f(
        size.x / res.x,
        size.y / res.y
    );

    sprite.setScale(scale);
    sprite.setOrigin(res / 2.0f);
}

void AStarOverlay::setResolution(sf::RenderTexture& texture, sf::Vector2u resolution) const {

    if (!texture.resize(resolution)) {
        throw std::runtime_error("Failed to resize texture!");
    }
}

void AStarOverlay::setView(const sf::View& view) {

    textures[0].setView(view);
    textures[1].setView(view);
}

void AStarOverlay::setPosition(const sf::Vector2f& position) {

    sprite.setPosition(position);
}

void AStarOverlay::setRotation(float rotation) {

    sprite.setRotation(sf::degrees(rotation));
}

void AStarOverlay::draw(sf::RenderTarget& target) {

    textures[index].clear(sf::Color(0, 255, 0, 128));
    textures[index].display();

    sprite.setTexture(textures[index].getTexture(), true);

    target.draw(sprite);

    if (index == 0)
        index = 1;
    else
        index = 0;
}