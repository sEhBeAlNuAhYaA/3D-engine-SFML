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

enum DrawableType
{
    ISSHAPE,
    ISSPRITE
};

class Drawable
{
public:
    Drawable() {}
    Drawable(long newId);

public:
    void SetShape(std::shared_ptr<sf::Shape> newShape);
    void SetTextureAndSprite(std::shared_ptr<sf::Texture> newTexture, sf::IntRect textureRect, sf::Vector2f Scale, sf::Vector2f textureOrigin, sf::Vector2f position);
    void SetSpriteOrigin(const sf::Vector2f& origin);
    void SetSpritePosition(const sf::Vector2f& position);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetPoint(int point) const;
    std::shared_ptr<sf::Shape> GetShape() const;
    std::shared_ptr<sf::Sprite> GetSprite() const;
    DrawableType GetDrawableType() const;

private:
    DrawableType m_drawableType;
    std::shared_ptr<sf::Shape> m_shape = 0;
    std::shared_ptr<sf::Sprite> m_sprite = 0;
    std::shared_ptr<sf::Texture> m_texture = 0;
    long m_id;

};

class DrawableCollection
{
public:
    DrawableCollection(){}
    void PushDrawable(std::shared_ptr<Drawable> drawable_ptr);
    void PushDrawablePixelForMap(std::shared_ptr<Drawable> drawable_ptr);
    std::vector<std::shared_ptr<Drawable>> GetDrawableColection();
    std::vector<std::shared_ptr<Drawable>> GetDrawablePixelsForMapColection();

private:
    std::vector<std::shared_ptr<Drawable>> m_drawableDrawableCollection;

public:
    std::vector<std::shared_ptr<std::array<sf::Vertex, 2>>> mapDrawable;
    std::vector<std::shared_ptr<Drawable>> m_mapPixels;
};


