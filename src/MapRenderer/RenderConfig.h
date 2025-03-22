#pragma once

#include "../Map/RoadType.h"

#include <SFML/Graphics/Color.hpp>

namespace asc2 {

    struct RenderConfig {

        bool displayIncomplete = false;
        bool buildingLineMode = false;

        sf::Color backgroundColor = sf::Color::Black;
        sf::Color roadColor       = sf::Color::White;
        std::map<RoadType, sf::Color> roadColors {};
    };
}