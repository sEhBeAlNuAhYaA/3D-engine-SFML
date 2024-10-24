#include "EntityManager.h"

Entity::Entity(sf::Shape* newShape, long newId)
{
    //m_shape = newShape;
    m_id = newId;
}

void Entity::SetShape(std::shared_ptr<sf::Shape> newShape)
{
    m_shape = newShape;
}

void Entity::SetTexture(std::shared_ptr<sf::Texture> newTexture, sf::IntRect textureRect, sf::Vector2f scale)
{
    m_sprite = std::make_shared<sf::Sprite>();
    m_texture = newTexture;
    m_sprite->setTexture(*m_texture);
    m_sprite->setTextureRect(textureRect);
    m_sprite->setScale(scale);
    m_sprite->setOrigin(0,32);
    m_sprite->setPosition(m_shape->getPosition());
}

sf::Vector2f Entity::GetPosition()
{
    return m_shape->getPosition();
}

sf::Vector2f Entity::GetPoint(int point)
{
    return m_shape->getPoint(point);
}

std::shared_ptr<sf::Shape> Entity::GetShape()
{
    return m_shape;
}

std::shared_ptr<sf::Sprite> Entity::GetSprite()
{
    return m_sprite;
}

void DrawableEntityCollection::PushEntity(std::shared_ptr<Entity> entity_ptr)
{
    m_drawableEntityCollection.push_back(entity_ptr);
}

void DrawableEntityCollection::PushEntityPixelForMap(std::shared_ptr<Entity> entity_ptr)
{
    m_mapPixels.push_back(entity_ptr);
}

std::vector<std::shared_ptr<Entity>> DrawableEntityCollection::GetEntityColection()
{
    return m_drawableEntityCollection;
}

std::vector<std::shared_ptr<Entity>> DrawableEntityCollection::GetEntityPixelsForMapColection()
{
    return m_mapPixels;
}

