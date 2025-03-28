
#include "AStarOverlay.h"

#include <iostream>

using namespace asc2;

AStarOverlay::AStarOverlay(const sf::Vector2u& size) :
    sprite1(textures[0].getTexture()),
    sprite2(textures[1].getTexture()) {

    setResolution(size);
}

void AStarOverlay::setResolution(sf::Vector2u resolution) {

    setResolution(textures[0], resolution);
    setResolution(textures[1], resolution);

    sprite1.setTexture(textures[0].getTexture(), true);
    sprite2.setTexture(textures[1].getTexture(), true);
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

    sprite1.setScale(scale);
    sprite2.setScale(scale);
    sprite1.setOrigin(res / 2.0f);
    sprite2.setOrigin(res / 2.0f);
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

void AStarOverlay::clear() {

    textures[0].clear(sf::Color::Transparent);
    textures[1].clear(sf::Color::Transparent);

    textures[0].display();
    textures[1].display();
}

void AStarOverlay::setPosition(const sf::Vector2f& position) {

    sprite1.setPosition(position);
    sprite2.setPosition(position);
}

void AStarOverlay::setRotation(float rotation) {

    sprite1.setRotation(sf::degrees(rotation));
    sprite2.setRotation(sf::degrees(rotation));
}

void AStarOverlay::draw(sf::RenderTarget& target) {

    sf::RenderTexture& currentTexture = textures[index];
    sf::Sprite& currentSprite = index == 0 ? sprite1 : sprite2;

    index = index == 0 ? 1 : 0;

    sf::RenderTexture& nextTexture = textures[index];
    sf::Sprite& nextSprite = index == 0 ? sprite1 : sprite2;

    nextTexture.clear(sf::Color::Transparent);
    nextTexture.draw(currentSprite);
    nextTexture.display();

    target.draw(nextSprite);
}