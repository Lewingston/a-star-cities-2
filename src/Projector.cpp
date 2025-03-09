
#include "Projector.h"
#include "Map/Map.h"

#include <numbers>
#include <cmath>

using namespace asc2;

constexpr double PI        = std::numbers::pi_v<double>;
constexpr double EARTH_RAD = 6'378'137.0;

void Projector::project(Map& map) {

    const std::pair<double, double> center = map.getCenter();

    auto func = [&](double lon, double lat) -> std::pair<double, double> {

        const double posY = -distance(lat - center.second, EARTH_RAD);
        const double posX = distance(lon - center.first, getRadAtLat(lat));

        return {posX, posY};
    };

    map.applyProjection(func);
}

double Projector::distance(double angel, double radius) {

    return radius * PI / 180.0 * angel;
}

double Projector::getRadAtLat(double lat) {

    return std::sin(lat * PI / 180.0) * EARTH_RAD;
}