#pragma once
#include "SFML/Graphics.hpp"
#include "Tools.h"

class Button
{
public:
    Button(std::shared_ptr<sf::Text> text);
    bool IsCursorOnButton(const sf::Vector2i& cursorPosition);

public:
    std::shared_ptr<sf::Text> m_text;
};

