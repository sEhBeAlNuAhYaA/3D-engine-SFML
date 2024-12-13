#include "TextureLoader.h"

TextureLoader::TextureLoader(std::string fileName)
{
    m_textureDBFileName = fileName;
    printf("LOADING TEXTURES FROM FILE DB - %s\n", fileName.c_str());
    std::ifstream filedb;
    filedb.open(m_textureDBFileName);
    std::string line;
    
    if (filedb.is_open())
    {
        while (getline(filedb, line))
        {
            if (line == SPRITES)
            {
                currentTextureState = ISSPRITETEX;
                continue;
            }
            sf::Texture texture;
            texture.loadFromFile(line.substr(1, line.size()));
            if (currentTextureState == ISNOTSPRITETEX)
            {
                m_Textures.insert(std::make_pair(line[0], std::make_shared<sf::Texture>(texture)));
            }
            if (currentTextureState == ISSPRITETEX)
            {
                m_spriteTextures.insert(std::make_pair(line[0], std::make_shared<sf::Texture>(texture)));
            }
        }
    }
    else
    {
        ERROR("FILE WASNT OPEN");
    }
    int a;
}

std::shared_ptr<sf::Texture> TextureLoader::GetTextureByMap(const char& mapObject)
{
    return m_Textures[mapObject];
}

std::shared_ptr<sf::Texture> TextureLoader::GetSpriteTexture(const char& Object)
{
    return m_spriteTextures[Object];
}