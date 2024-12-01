#include "Player.h"

PlayerOnMap::PlayerOnMap()
{
}

PlayerOnMap::PlayerOnMap(const int fov)
{
    m_FOV = fov;
    m_playerShape.setRadius(2.0);
    m_playerShape.setOrigin(2.0, 2.0);
    m_playerShape.setPosition(DISTANCE + 5.f, DISTANCE + 5.f);
    playerViewDirection = 0.1; 
    m_playerDirection = Forward;
}
