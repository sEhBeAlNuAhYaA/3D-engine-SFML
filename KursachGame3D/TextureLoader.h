#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Tools.h"

enum textureParts
{
    ISNOTSPRITETEX,
    ISSPRITETEX
};

class TextureLoader
{
public:
    TextureLoader(std::string fileName);
    std::shared_ptr<sf::Texture> GetTextureByMap(const char& mapObject);
    std::shared_ptr<sf::Texture> GetSpriteTexture(const char& mapObject);

private:
    std::map<char, std::shared_ptr<sf::Texture>> m_Textures;
    std::map<char, std::shared_ptr<sf::Texture>> m_spriteTextures;
    std::string m_textureDBFileName;
    textureParts currentTextureState = ISNOTSPRITETEX;
};

