#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Map.h"
#include "DrawableManager.h"
#include "Player.h"
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>
#include <array>
#include "Tools.h"
#include "TextureLoader.h"
#include "Entity.h"

static char m_Map[50][50];

class RayCastingProccessingForMapAndFrame
{
public:
    RayCastingProccessingForMapAndFrame(const PlayerOnMap& player);

public:
    DrawableCollection FillEntitiesCollectionForMapAndFrame();

public:
    std::vector<std::shared_ptr<sf::Shape>> GetMap();
    void FillShapesMap();

public:
    sf::Vector2f GetPlayerPositionFromMap();
    void setPlayerOnMapPostion(const sf::Vector2f& position);
    void PlayerMovement(double deltaTime);
    void PlayerCollisionsWithMap(const sf::Vector2f& playerPosition, double deltaTime);
    PlayerOnMap GetPlayerOnMap();

public:
    void addMapBorder(DrawableCollection& entitiesCollection);
    void addBackGroundShape(DrawableCollection& entitiesCollection);
    void addArms(DrawableCollection& entitiesCollection);  

public:
    std::vector<Entity> m_EntityList;

public:
    void fillEntityFromEngine(DrawableCollection& drawableCollection, std::map<double, sf::Vector2f> wallParams);

private:
    std::vector<std::shared_ptr<sf::Shape>> m_ShapesMap;
    int m_playerOnMapPosition;
    TextureLoader m_textureLoader;
    PlayerOnMap m_playerOnMap;
};
