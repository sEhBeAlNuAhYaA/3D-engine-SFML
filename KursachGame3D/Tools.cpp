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

    float Interpolation(const sf::Vector2f& firstPoint, const sf::Vector2f& secondPoint, float Xposition)
    {
        return (firstPoint.y + (secondPoint.y - firstPoint.y) * (Xposition - firstPoint.x) / (secondPoint.x - firstPoint.x));
    }

    std::shared_ptr<sf::Text> createText(const sf::Font& font, const std::string string, const sf::Vector2f scale, unsigned int size, const sf::Vector2f position)
    { 
        std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>();
        text->setFont(font);
        text->setString(string);
        text->setScale(scale);
        text->setCharacterSize(size);
        text->setPosition(position);
        return text;
    }
}