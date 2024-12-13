#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
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
#include "queue"


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
    bool isValidPosition(int x, int y, std::vector<std::vector<bool>>& visited);

public:
    sf::Vector2f GetPlayerPositionFromMap();
    void setPlayerOnMapPostion(const sf::Vector2f& position);
    void PlayerMovement(sf::Clock& globalClock, sf::Event& event, sf::Clock& playerHitClock, sf::RenderWindow& m_window);
    void PlayerCollisionsWithMap(const sf::Vector2f& playerPosition, double deltaTime);
    PlayerOnMap& GetPlayerOnMap();
    void DoHit(sf::Clock& clock);
    void checkForKills();
    void playerDamage(sf::Clock& clockForEntity);
    bool IsPlayerDead();

public:
    void addMapBorder(DrawableCollection& entitiesCollection);
    void addBackGroundShape(DrawableCollection& entitiesCollection);
    void addArms(DrawableCollection& entitiesCollection);  
    void addHud(DrawableCollection& entitiesCollection);

public:
    std::vector<Entity> m_EntityList;

public:
    void fillEntityFromEngine(DrawableCollection& drawableCollection, std::map<double, sf::Vector2f> wallParams);
    void FindPathForEveryEntity();
    std::vector<sf::Vector2i> FindShortestPath(const sf::Vector2f& entityPosition, const sf::Vector2f& playerPosition);
    void MoveEntityToPlayer(sf::Clock& clock, double deltaTime);

private:
    std::vector<std::shared_ptr<sf::Shape>> m_ShapesMap;
    int m_playerOnMapPosition;
    TextureLoader m_textureLoader;
    PlayerOnMap m_playerOnMap;
    sf::Music m_hitSound;
    sf::Music m_killSound;
};
