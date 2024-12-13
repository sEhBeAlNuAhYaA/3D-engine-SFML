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
    m_playerDirection = Direction::Forward;
    m_bisHiting = false;
    m_HP = 100;
    m_damage = 25;
    m_Armor = 100;
    m_entityDamage = 10;
    m_killsCounter = sf::Vector2i(0,0);
    m_gun = Gun::Pistol;
    m_lvl = Lvl::lvl0;
    m_score = 0;

}

void PlayerOnMap::updateLvl()
{
    if (m_lvl == Lvl::lvl0)
    {
        m_Armor = 0;
        m_gun = Gun::Pistol;
    }
    if (m_lvl == Lvl::lvl1)
    {
        m_entityDamage = 25;
        m_Armor = 25;
        m_gun = Gun::Pistol;
    }
    if (m_lvl == Lvl::lvl2)
    {
        m_entityDamage = 40;
        m_Armor = 50;
        m_gun = Gun::Shootgun;
    }
    if (m_lvl == Lvl::lvl3)
    {
        m_entityDamage = 70;
        m_Armor = 100;
        m_gun = Gun::Rifle;
    }
}

void PlayerOnMap::updateGun()
{
    if (m_gun == Gun::Pistol)
    {
        m_damage = 20;
    }
    if (m_gun == Gun::Shootgun)
    {
        m_damage = 50;
    }
    if (m_gun == Gun::Rifle)
    {
        m_damage = 100;
    }
}

