#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>
#include "Tools.h"


enum Direction
{
    Forward,
    Backward,
    Left,
    Right
};

struct PlayerOnMap
{
public:
    PlayerOnMap();
    PlayerOnMap(const int fov);

public:
    int m_FOV;
    sf::CircleShape m_playerShape;
    Direction m_playerDirection;
    double playerViewDirection;
};