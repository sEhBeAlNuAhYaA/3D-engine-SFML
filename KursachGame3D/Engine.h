#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Map.h"
#include "DrawableManager.h"
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>
#include "Tools.h"
#include "TextureLoader.h"
#include "Button.h"


enum class GameState
{
    MainMenu,
    ShopMenu,
    Game
};

class Engine
{
public:
    Engine();
    ~Engine();
    void DoRenderMainWindow();
    void DrawMap(RayCastingProccessingForMapAndFrame* map);
    void PoolEvent(const sf::Event& eventType);
    void Update();
    void DrawEntities(DrawableCollection entitiesCollection);
    void DrawTab(const sf::Event& event);
    void DrawMainMenu(const sf::Event& event);
    void DrawShop(const sf::Event& event);
    void DrawGame(const sf::Event& event);
    void DrawEnemiesCounter();
    void DrawBulletsCounter();

private:
    sf::RenderWindow m_window;
    sf::Event m_event;
    RayCastingProccessingForMapAndFrame* m_RayCastingProccessingForMapAndFrame;
    sf::Clock m_globalClock;
    sf::Clock m_entityClock;
    sf::Clock m_entityHitClock;
    sf::Clock m_playerHitClock;
    sf::Music m_backMusic;
    std::shared_ptr<sf::Font> m_font = 0;
    GameState m_gameState;
    PlayerOnMap* m_playerOnMap;
};