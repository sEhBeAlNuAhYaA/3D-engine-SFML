#include "Tools.h"
namespace Tools
{
    sf::Color SetTransparetForColor(const sf::Color& color, unsigned int transparent)
    {
        return sf::Color(color.r, color.g, color.b, transparent);
    }
    float MinMax(float x, float xmax, float xmin, float a, float b)
    {
        return a + (((x - xmin) / (xmax - xmin)) * (b - a));
    }
}