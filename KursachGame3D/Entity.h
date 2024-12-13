#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "DrawableManager.h"
#include "Tools.h"

enum class EntityDirection
{
    Forward,
    Backward,
    Left,
    Right
};

class Entity
{
public:
    Entity(sf::Vector2f position, float HP);
    Entity(
        float hp, 
        float damage, 
        float animationTime, 
        sf::Vector2f startPosition, 
        std::string textureFileName, 
        sf::IntRect textureRect, 
        sf::Vector2f scale, 
        sf::Vector2f origin);
public:
    void FillEntity(
        float damage,
        float animationTime,
        sf::IntRect textureRect,
        sf::Vector2f scale,
        sf::Vector2f origin,
        sf::Vector2f onScreenPosition);
    void SetTexture(std::string textureFileName);
    void SetImage(std::string imageName);
    void entityCollisionsAndLoadingTexture(std::map<double, sf::Vector2f> wallTiles, sf::Vector2f origin, sf::Vector2f scale, sf::Vector2f onScreenPostion);
    void SetSpritePositionOnScreen(const sf::Vector2f& position);
    void SetSpriteOriginOnScreen(const sf::Vector2f& origin);

public:
    float m_HP;
    float m_Damage;
    float m_AnimationTime;
    float m_Distance;
    long  m_id;
    sf::Vector2f m_CurrentPosition;
    std::shared_ptr<sf::Texture> m_Texture = 0;
    sf::Image m_Image;
    std::shared_ptr<Drawable> m_DrawableSprite = 0;
    std::vector<sf::Vector2i> m_pathToPlayer;
    EntityDirection m_Direction = EntityDirection::Forward;
};

