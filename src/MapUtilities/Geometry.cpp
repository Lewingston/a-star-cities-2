
#include "Geometry.h"

using namespace asc2;

bool Geometry::pointInRectangle(sf::Vector2f point, const Rectangle& rectangle) {

    const sf::Vector2f AB = rectangle.pointB() - rectangle.pointA();
    const sf::Vector2f AM = point - rectangle.pointA();
    const sf::Vector2f BC = rectangle.pointC() - rectangle.pointB();
    const sf::Vector2f BM = point - rectangle.pointB();

    const float dotABAM = AB.dot(AM);
    const float dotABAB = AB.dot(AB);
    const float dotBCBM = BC.dot(BM);
    const float dotBCBC = BC.dot(BC);

    return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;
}