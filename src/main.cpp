
#include "AStarCities.h"
#include "MapParser.h"
#include "Projector.h"
#include "Map/Map.h"
#include "MapRenderer/LineBuffer.h"

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>

using namespace asc2;

int main(int argc, const char** args) {

    std::unique_ptr<Map> map;

    try {
        MapParser parser("../../maps/budesheim.json");
        map = parser.parse();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    Projector::project(*map);

    LineBuffer lineBuffer(map->getAllWays());

    sf::RenderWindow window(sf::VideoMode({800, 600}), ProgramInfo::name);

    sf::View view = window.getDefaultView();
    window.setView(view);
    view.setCenter(sf::Vector2f{map->getCenter().first, map->getCenter().second});

    auto windowSize = window.getSize();

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {

            if (!event.has_value())
                continue;

            if (event->is<sf::Event::Closed>()) {

                window.close();

            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {

                auto view = window.getView();
                view.setSize(sf::Vector2f(resized->size));
                window.setView(view);

                windowSize = resized->size;
            }
        }

        window.clear(sf::Color::White);

        lineBuffer.draw(window);

        window.display();
    }

    return 0;
}