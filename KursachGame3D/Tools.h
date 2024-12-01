#pragma once
#include "SFML/Graphics.hpp"

#define BASE_WIDTH 1024
#define BASE_HEIGHT 768
#define PLAYERSPEED 120
#define ROTATESPEED 3.5
#define VIEWANGLE 1.57
#define DISTANCE 150
#define MapObjectWidth 16
#define MapObjectHeght 16

#define ERROR(x) printf("ERROR - %s", x)
#define LOG(x) printf("MESSAGELOG - %s", x)

#define SPRITES "SPRITES"

static int FOV = 32;

namespace Tools
{
    sf::Color SetTransparetForColor(const sf::Color& color, unsigned int transparent);

    float MinMax(float x, float xmax, float xmin, float a, float b);
}