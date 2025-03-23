#pragma once

#include "../Map/RoadType.h"

#include <SFML/Graphics/Color.hpp>

namespace asc2 {

    struct RenderConfig {

        bool displayIncomplete = false;
        bool buildingLineMode  = false;
        bool randomColors      = false;

        bool renderAllLines      = true;
        bool renderRoads         = false;
        bool renderBuildings     = false;
        bool renderIntersections = false;

        sf::Color backgroundColor = sf::Color::White;
        sf::Color roadColor       = sf::Color::Black;
        std::map<RoadType, sf::Color> roadColors {};
    };
}