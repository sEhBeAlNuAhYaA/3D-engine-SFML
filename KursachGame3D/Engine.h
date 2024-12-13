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

class Engine
{
public:
    Engine();
    Engine(const int framerate, const int fov);
    ~Engine();
    void DoRenderMainWindow();
    void DrawMap(RayCastingProccessingForMapAndFrame* map);
    void PoolEvent(const sf::Event& eventType);
    void Update();
    void DrawEntities(DrawableCollection entitiesCollection);
    void DrawTab(const sf::Event& event);

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
};