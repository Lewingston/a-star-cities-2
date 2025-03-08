
#include "AStarCities.h"
#include "MapParser.h"

#include <iostream>

#include <SFML/Window.hpp>

using namespace asc2;

int main(int argc, const char** args) {

    try {
        MapParser parser;
        parser.loadFromFile("../../bingen.json");
        parser.parse();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    /*sf::Window window(sf::VideoMode({800, 600}), ProgramInfo::name);

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {

            if (!event.has_value())
                continue;

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
    }*/

    return 0;
}