#pragma once
#include "SFML/Graphics.hpp"
#include <fstream>

#define BASE_WIDTH 1024
#define BASE_HEIGHT 768
#define PLAYERSPEED 120
#define BOTSPEED 5
#define ROTATESPEED 5
#define VIEWANGLE 1.57
#define DISTANCE 150
#define MapObjectWidth 16
#define MapObjectHeght 16

#define MAP_HEIGHT 30
#define MAP_WIDTH 30


#define ERROR(x)// log << "ERROR - %s" + x << "\n";
#define LOG(x) //log << "MESSAGELOG - %s" + x << "\n";

#define SPRITES "SPRITES"

static int FOV = 32;

namespace Tools
{
    sf::Color SetTransparetForColor(const sf::Color& color, unsigned int transparent);

    float MinMax(float x, float xmax, float xmin, float a, float b);

    float Interpolation(const sf::Vector2f& firstPoint, const sf::Vector2f& secondPoint, float position);

    std::shared_ptr<sf::Text> createText(const sf::Font& font, const std::string string, const sf::Vector2f scale, unsigned int size, const sf::Vector2f position);
}