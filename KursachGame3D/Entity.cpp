#include "Entity.h"

Entity::Entity(sf::Vector2f position, float HP)
    : m_CurrentPosition(position), m_HP(HP)
{
}

Entity::Entity(
    float hp, 
    float damage, 
    float animationTime, 
    sf::Vector2f startPosition,
    std::string textureFileName,
    sf::IntRect textureRect, 
    sf::Vector2f scale, 
    sf::Vector2f origin)
    : m_HP(hp), m_Damage(damage), m_AnimationTime(animationTime) 
{
    m_Texture->loadFromFile(textureFileName);
    m_DrawableSprite->SetTextureAndSprite(m_Texture, textureRect, scale, origin, startPosition);
}

void Entity::FillEntity(
    float damage,
    float animationTime,
    sf::IntRect textureRect,
    sf::Vector2f scale,
    sf::Vector2f origin,
    sf::Vector2f onScreenPosition)
{
    m_Damage = damage;
    m_AnimationTime = animationTime;
    Drawable draw;
    draw.SetTextureAndSprite(m_Texture, textureRect, scale, origin, onScreenPosition);
    m_DrawableSprite = std::make_shared<Drawable>(draw);
}

void Entity::SetTexture(std::string textureFileName)
{
    sf::Texture tex;
    tex.loadFromFile(textureFileName);
    m_Texture = std::make_shared<sf::Texture>(tex);
}

void Entity::SetImage(std::string imageName)
{
    m_Image.loadFromFile(imageName);
}

void Entity::entityCollisionsAndLoadingTexture(std::map<double, sf::Vector2f> wallTiles, sf::Vector2f origin, sf::Vector2f scale, sf::Vector2f onScreenPostion)
{
    int spriteLeftBound = onScreenPostion.x - (m_Image.getSize().x) * scale.x / 2;
    int spriteRightBound = onScreenPostion.x + (m_Image.getSize().x) * scale.x / 2;

    sf::Image tempImage = m_Image;
    sf::Texture tex;

    if (spriteLeftBound < 0) spriteLeftBound = 0;
    if (spriteRightBound > BASE_WIDTH) spriteRightBound = BASE_WIDTH;
    auto lastPoint = -1;

    if (m_Distance < DISTANCE)
    {
        for (auto it = std::begin(wallTiles), end = std::end(wallTiles); it != end; ++it)
        {
            if (it->second.x > spriteLeftBound && it->second.x < spriteRightBound && it->first < m_Distance)
            {
                if (lastPoint < 0)
                {
                    lastPoint = (int)Tools::MinMax(it->second.x, spriteRightBound, spriteLeftBound, 0, m_Image.getSize().x);
                    continue;
                }

                auto currentPoint = (int)Tools::MinMax(it->second.x, spriteRightBound, spriteLeftBound, 0, m_Image.getSize().x);

                for (int i = 0; i <= abs(currentPoint - lastPoint); ++i)
                {
                    for (int j = 0; j < m_Image.getSize().y; ++j)
                    {
                        if (lastPoint + i >= m_Image.getSize().x) lastPoint = m_Image.getSize().x - i - 1;
                        sf::Color pixel = tempImage.getPixel(lastPoint + i, j);
                        pixel.a = 0;
                        tempImage.setPixel(lastPoint + i, j, pixel);
                    }
                }
                lastPoint = currentPoint;
            }
        }
    }
    
    tex.loadFromImage(tempImage);
    m_Texture = std::make_shared<sf::Texture>(tex);
    FillEntity(
        10,
        0,
        sf::IntRect(0, 0, m_Image.getSize().x, m_Image.getSize().y),
        scale,
        origin,
        onScreenPostion);
    m_DrawableSprite->GetSprite()->setColor(Tools::SetTransparetForColor(m_DrawableSprite->GetSprite()->getColor(), m_Distance >= DISTANCE ? 0 : 255 - m_Distance * 255 / DISTANCE));
    
    if (m_Distance > DISTANCE)
    {
        m_DrawableSprite->GetSprite()->setColor(sf::Color(0, 0, 0, 0));
    }
}

void Entity::SetSpritePositionOnScreen(const sf::Vector2f& position)
{
    m_DrawableSprite->SetSpritePosition(position);
}

void Entity::SetSpriteOriginOnScreen(const sf::Vector2f& origin)
{
    m_DrawableSprite->SetSpriteOrigin(origin);
}


