#include "DrawableManager.h"

Drawable::Drawable(long newId)
{
    m_id = newId;
}

std::shared_ptr<sf::Shape> Drawable::GetShape() const
{
    return m_shape;
}

std::shared_ptr<sf::Sprite> Drawable::GetSprite() const
{
    return m_sprite;
}

void Drawable::SetShape(std::shared_ptr<sf::Shape> newShape)
{
    m_drawableType = DrawableType::ISSHAPE;
    m_shape = newShape;
}

void Drawable::SetTextureAndSprite(std::shared_ptr<sf::Texture> newTexture, sf::IntRect textureRect, sf::Vector2f scale, sf::Vector2f textureOrigin, sf::Vector2f position)
{
    m_drawableType = DrawableType::ISSPRITE;
    m_texture = newTexture;
    m_sprite = std::make_shared<sf::Sprite>();
    m_sprite->setTexture(*m_texture);
    m_sprite->setTextureRect(textureRect);
    m_sprite->setScale(scale);
    m_sprite->setOrigin(textureOrigin.x, textureOrigin.y);
    m_sprite->setPosition(position);
}

void Drawable::SetSpriteOrigin(const sf::Vector2f& origin)
{
    m_sprite->setOrigin(origin);
}

void Drawable::SetSpritePosition(const sf::Vector2f& position)
{
    m_sprite->setPosition(position);
}

sf::Vector2f Drawable::GetPosition() const
{
    return m_shape->getPosition();
}

sf::Vector2f Drawable::GetPoint(int point) const
{
    return m_shape->getPoint(point);
}

DrawableType Drawable::GetDrawableType() const
{
    return m_drawableType;
}

void DrawableCollection::PushDrawable(std::shared_ptr<Drawable> drawable_ptr)
{
    m_drawableDrawableCollection.push_back(drawable_ptr);
}

void DrawableCollection::PushDrawablePixelForMap(std::shared_ptr<Drawable> drawable_ptr)
{
    m_mapPixels.push_back(drawable_ptr);
}

std::vector<std::shared_ptr<Drawable>> DrawableCollection::GetDrawableColection()
{
    return m_drawableDrawableCollection;
}

std::vector<std::shared_ptr<Drawable>> DrawableCollection::GetDrawablePixelsForMapColection()
{
    return m_mapPixels;
}

