
#include "AStarOverlay.h"

#include <iostream>

using namespace asc2;

constexpr std::string_view fadeShaderCode = R"(

uniform bool fade;
uniform sampler2D texture;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float red = fade ? 1.0f / 255.0f : 0.0f;
    gl_FragColor = vec4(1.0f, 1.0f, 1.0f, pixel.a - red);
}
)";

AStarOverlay::AStarOverlay(const sf::Vector2u& size) :
    sprite1(textures[0].getTexture()),
    sprite2(textures[1].getTexture()) {

    setResolution(size);

    loadShaders();
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

    sf::Sprite& currentSprite = index == 0 ? sprite1 : sprite2;

    target.draw(currentSprite);
}

void AStarOverlay::flip() {

    sf::Sprite& currentSprite = index == 0 ? sprite1 : sprite2;

    index = index == 0 ? 1 : 0;

    sf::RenderTexture& nextTexture = textures[index];

    nextTexture.clear(sf::Color::Transparent);

    if (shadersAvailable) {

        currentFadeFrame++;

        fadeShader.setUniform("fade", currentFadeFrame >= fadeSpeed);
        nextTexture.draw(currentSprite, &fadeShader);

        if (currentFadeFrame >= fadeSpeed)
            currentFadeFrame = 0;

    } else {
        nextTexture.draw(currentSprite);
    }

    nextTexture.display();
}

void AStarOverlay::loadShaders() {

    if (!sf::Shader::isAvailable()) {
        std::cout << "System does not support shaders!";
        shadersAvailable = false;
        return;
    }

    shadersAvailable = true;

    if (!fadeShader.loadFromMemory(fadeShaderCode, sf::Shader::Type::Fragment)) {
        throw std::runtime_error("Failed to load fade shader!");
    }
}