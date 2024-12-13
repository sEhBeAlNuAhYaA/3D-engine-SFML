#include "Button.h"

Button::Button(std::shared_ptr<sf::Text> text)
{
    m_text = text;
}

bool Button::IsCursorOnButton(const sf::Vector2i& cursorPosition)
{
    auto top = m_text->getPosition().y + m_text->getLocalBounds().top;
    auto left = m_text->getPosition().x;
    auto width = m_text->getLocalBounds().width;
    auto height = m_text->getLocalBounds().height;

    return (cursorPosition.x > left && cursorPosition.x < width + left) && (cursorPosition.y > top && cursorPosition.y < height + top);
}
