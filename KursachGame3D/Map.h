#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>
#include <array>
#include "Tools.h"

static char m_Map[50][50];

class RayCastingProccessingForMapAndFrame
{
public:
    RayCastingProccessingForMapAndFrame(PlayerOnMap player);
    //RayCastingProccessingForMapAndFrame(std::ifstream fileMap);
public:
    std::vector<std::shared_ptr<sf::Shape>> GetMap();
    sf::Vector2f GetPlayerPositionFromMap();
    void setPlayerOnMapPostion(sf::Vector2f position);
    void FillShapesMap(PlayerOnMap player);
    void PlayerMovement(double deltaTime);
    void PlayerCollisionsWithMap(sf::Vector2f playerPosition, double deltaTime);
    void addMapBorder(DrawableEntityCollection& entitiesCollection);
    PlayerOnMap GetPlayerOnMap();
    DrawableEntityCollection FillEntitiesCollectionForMapAndFrame(sf::RenderWindow* window);

private:
    std::vector<std::shared_ptr<sf::Shape>> m_ShapesMap;
    int m_playerOnMapPosition;
    PlayerOnMap m_playerOnMap;
};
