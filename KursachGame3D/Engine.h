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
    void DrawMap(RayCastingProccessingForMapAndFrame map);
    void PoolEvent();
    void Update();
    void DrawEntities(DrawableCollection entitiesCollection);

private:
    sf::RenderWindow m_window;
    sf::Event m_event;
    RayCastingProccessingForMapAndFrame* m_RayCastingProccessingForMapAndFrame;
};