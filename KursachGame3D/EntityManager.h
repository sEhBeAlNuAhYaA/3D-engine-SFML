#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <fstream>

class GlobalProvider
{
public:
    GlobalProvider(){}
    long GetNewUniqueId()
    {
        return ++uniqieId;
    }

private:
    long uniqieId;
};

class Entity
{
public:
    Entity() {}
    Entity(sf::Shape* newShape, long newId);

public:
    void SetShape(std::shared_ptr<sf::Shape> newShape);
    void SetTexture(std::shared_ptr<sf::Texture> newTexture, sf::IntRect textureRect, sf::Vector2f Scale, sf::Vector2f textureOrigin);
    sf::Vector2f GetPosition();
    sf::Vector2f GetPoint(int point);
    std::shared_ptr<sf::Shape> GetShape();
    std::shared_ptr<sf::Sprite> GetSprite();

private:
    std::shared_ptr<sf::Shape> m_shape = 0;
    std::shared_ptr<sf::Sprite> m_sprite = 0;
    std::shared_ptr<sf::Texture> m_texture = 0;
    long m_id;

};

class DrawableEntityCollection
{
public:
    DrawableEntityCollection(){}
    void PushEntity(std::shared_ptr<Entity> entity_ptr);
    void PushEntityPixelForMap(std::shared_ptr<Entity> entity_ptr);
    std::vector<std::shared_ptr<Entity>> GetEntityColection();
    std::vector<std::shared_ptr<Entity>> GetEntityPixelsForMapColection();

private:
    std::vector<std::shared_ptr<Entity>> m_drawableEntityCollection;

public:
    std::vector<std::shared_ptr<std::array<sf::Vertex, 2>>> mapDrawable;
    std::vector<std::shared_ptr<Entity>> m_mapPixels;
};


