#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>
#include "Tools.h"


enum class Direction
{
    Forward,
    Backward,
    Left,
    Right
};

enum class Gun
{
    Shootgun,
    Pistol,
    Rifle
};

enum Lvl
{
    lvl1,
    lvl2,
    lvl3
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
    bool m_bisHiting;
    float m_HP;
    float m_Armor;
    sf::Vector2i m_killsCounter;
    int m_damage;
    Gun m_gun;
    Lvl m_lvl;

};