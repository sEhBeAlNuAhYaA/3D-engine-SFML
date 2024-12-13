#include "Engine.h"
std::ofstream logGame;

Engine::Engine()
    : m_window(
        sf::VideoMode(BASE_WIDTH, BASE_HEIGHT), 
        "Game", 
        sf::Style::Titlebar | sf::Style::Close,
        sf::ContextSettings(0U, 0U, 8, 0U, 1U, 0U, false))
{
    printf("1ENGINE CONSTRUCTOR - FRAME LIMIT 60 - RESOLUTION %dx%d\n", BASE_WIDTH, BASE_HEIGHT);
    m_RayCastingProccessingForMapAndFrame = new RayCastingProccessingForMapAndFrame(PlayerOnMap(512));
    m_window.setFramerateLimit(60);
    DoRenderMainWindow();
}

Engine::Engine(const int framerate, const int fov)
    : m_window(
        sf::VideoMode(BASE_WIDTH, BASE_HEIGHT), 
        "Game", 
        sf::Style::Titlebar | sf::Style::Close,
        sf::ContextSettings(0U, 0U, 8, 0U, 1U, 0U, false))
{
    m_window.setMouseCursorVisible(false);
    logGame.open("game.log");
    logGame << "2ENGINE CONSTRUCTOR - FRAME LIMIT";
    m_RayCastingProccessingForMapAndFrame = new RayCastingProccessingForMapAndFrame(PlayerOnMap(fov));
    m_window.setFramerateLimit(framerate);
    DoRenderMainWindow();
}

Engine::~Engine()
{
    printf("ENGINE DESTRUCTOR\n");
    delete(m_RayCastingProccessingForMapAndFrame);
}

void Engine::DoRenderMainWindow()
{
    m_backMusic.openFromFile("bMusic.mp3");
    m_backMusic.setLoop(true);
    m_backMusic.setVolume(1);
    m_backMusic.play();
    m_font = std::make_shared<sf::Font>();
    m_font->loadFromFile("font.ttf");
    Update();
}


void Engine::Update()
{
    
    sf::Mouse::setPosition(sf::Vector2i(BASE_WIDTH / 2, BASE_HEIGHT / 2), m_window);
    try {
        while (m_window.isOpen())
        {
            
            
            while (m_window.pollEvent(m_event))
            {
                PoolEvent(m_event);
            }

            m_window.clear();
            if (m_window.hasFocus())
            { 
                if (m_RayCastingProccessingForMapAndFrame->IsPlayerDead())
                {
                    //DrawMenu();
                    break;
                }

                m_RayCastingProccessingForMapAndFrame->PlayerMovement(m_globalClock, m_event, m_playerHitClock, m_window);
                m_RayCastingProccessingForMapAndFrame->MoveEntityToPlayer(m_entityClock, m_entityClock.getElapsedTime().asSeconds());
                m_RayCastingProccessingForMapAndFrame->playerDamage(m_entityHitClock);

                m_RayCastingProccessingForMapAndFrame->checkForKills();              
            }
            else
            {
                m_playerHitClock.restart();
                m_entityHitClock.restart();
                m_entityClock.restart();
                m_globalClock.restart();
            }
            DrawableCollection frame = m_RayCastingProccessingForMapAndFrame->FillEntitiesCollectionForMapAndFrame();
            //drawing frame
            DrawEntities(frame);
            DrawMap(m_RayCastingProccessingForMapAndFrame);
            m_window.display();
        }
    }
    catch (...)
    {

    }
}

void Engine::DrawEntities(DrawableCollection entitiesCollection)
{
    for (auto it : entitiesCollection.GetDrawableColection())
    {
        if (it->GetDrawableType() == DrawableType::ISSHAPE)
        {
            m_window.draw(*(it.get()->GetShape()));
        }
        else if (it->GetDrawableType() == DrawableType::ISSPRITE)
        {
            m_window.draw(*(it.get()->GetSprite()));
        }
    }

    for (auto it = entitiesCollection.mapDrawable.begin(), end = entitiesCollection.mapDrawable.end(); it != end; ++it)
    {
        m_window.draw(it->get()->data(), 2, sf::Lines);
    }

    for (auto it : entitiesCollection.GetDrawablePixelsForMapColection())
    {
        m_window.draw(*(it.get()->GetShape()));
    }

    entitiesCollection.GetDrawableColection().clear();
}

void Engine::DrawTab(const sf::Event& event)
{
    m_window.setMouseCursorVisible(true);
    std::shared_ptr<sf::RectangleShape> back = std::make_shared<sf::RectangleShape>(sf::Vector2f(BASE_WIDTH, BASE_HEIGHT));
    back->setFillColor(sf::Color(20, 0, 0));
    std::shared_ptr<sf::RectangleShape> line = std::make_shared<sf::RectangleShape>(sf::Vector2f(3, 420));
    line->setFillColor(sf::Color::White);
    line->setPosition(sf::Vector2f(300, 60));
    //Stats
    std::shared_ptr<sf::Text> MenuName = Tools::createText(*m_font, "Menu", sf::Vector2f(1, 1), 60, sf::Vector2f(50, 50));
    std::shared_ptr<sf::Text> StatsText = Tools::createText(*m_font, "Stats:", sf::Vector2f(1, 1), 50, sf::Vector2f(50, 120));
    std::shared_ptr<sf::Text> KillsText = Tools::createText(*m_font, "Kills: " + std::to_string(m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_killsCounter.y), sf::Vector2f(1, 1), 40, sf::Vector2f(50, 175));
    std::shared_ptr<sf::Text> SpeedText = Tools::createText(*m_font, "Speed: " + std::to_string(PLAYERSPEED), sf::Vector2f(1, 1), 40, sf::Vector2f(50, 210));
    std::shared_ptr<sf::Text> DamageText = Tools::createText(*m_font, "Damage: " + std::to_string(m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_damage), sf::Vector2f(1, 1), 40, sf::Vector2f(50, 245));
    std::shared_ptr<sf::Text> LvlText = Tools::createText(*m_font, "Lvl: " + std::to_string(m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl + 1), sf::Vector2f(1, 1), 40, sf::Vector2f(50, 280));

    //exit
    std::shared_ptr<Button> Exit = std::make_shared<Button>(Tools::createText(*m_font, "Exit", sf::Vector2f(1, 1), 50, sf::Vector2f(50, 400)));

    //guns
    std::shared_ptr<Button> PistolButton = std::make_shared<Button>(Tools::createText(*m_font, "PISTOL", sf::Vector2f(1, 1), 100, sf::Vector2f(BASE_WIDTH / 3, 40)));
    std::shared_ptr<Button> ShootGunButton = std::make_shared<Button>(Tools::createText(*m_font, "SHOOTGUN", sf::Vector2f(1, 1), 100, sf::Vector2f(BASE_WIDTH / 3, 200)));
    std::shared_ptr<Button> RifleButton = std::make_shared<Button>(Tools::createText(*m_font, "RIFLE", sf::Vector2f(1, 1), 100, sf::Vector2f(BASE_WIDTH / 3, 360)));

    m_backMusic.pause();
    while (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {   
        m_playerHitClock.restart();
        m_entityHitClock.restart();
        m_entityClock.restart();
        m_globalClock.restart();

        if (Exit->IsCursorOnButton(sf::Mouse::getPosition(m_window)))
        {
            Exit->m_text->setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_window.close();
            }
        }
        else
        {
            Exit->m_text->setFillColor(sf::Color::White);
        }

        if (PistolButton->IsCursorOnButton(sf::Mouse::getPosition(m_window)))
        {
            PistolButton->m_text->setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun = Gun::Pistol;
            }
        }
        else
        {
            m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun == Gun::Pistol ? PistolButton->m_text->setFillColor(sf::Color::Red) : PistolButton->m_text->setFillColor(sf::Color::White);
        }
        if (ShootGunButton->IsCursorOnButton(sf::Mouse::getPosition(m_window)) && (m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl == Lvl::lvl2 || m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl == Lvl::lvl3))
        {
            ShootGunButton->m_text->setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun = Gun::Shootgun;
            }
        }
        else if (m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl != Lvl::lvl2 && m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl != Lvl::lvl3)
        {
            ShootGunButton->m_text->setFillColor(sf::Color(127, 127, 127));
        }
        else
        {
            m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun == Gun::Shootgun ? ShootGunButton->m_text->setFillColor(sf::Color::Red) : ShootGunButton->m_text->setFillColor(sf::Color::White);
        }

        if (RifleButton->IsCursorOnButton(sf::Mouse::getPosition(m_window)) && m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl == Lvl::lvl3)
        {
            RifleButton->m_text->setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun = Gun::Rifle;
            }
        }
        else if (m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_lvl != Lvl::lvl3)
        {
            RifleButton->m_text->setFillColor(sf::Color(127, 127, 127));
        }
        else
        {
            m_RayCastingProccessingForMapAndFrame->GetPlayerOnMap().m_gun == Gun::Rifle ? RifleButton->m_text->setFillColor(sf::Color::Red) : RifleButton->m_text->setFillColor(sf::Color::White);
        }

        m_window.clear();
        m_window.draw(*back);
        m_window.draw(*MenuName);
        m_window.draw(*StatsText);
        m_window.draw(*KillsText);
        m_window.draw(*SpeedText);
        m_window.draw(*DamageText);
        m_window.draw(*LvlText);
        m_window.draw(*(PistolButton->m_text));
        m_window.draw(*(ShootGunButton->m_text));
        m_window.draw(*(RifleButton->m_text));
        m_window.draw(*line);
        m_window.draw(*(Exit->m_text));
        m_window.display();
    }
    m_backMusic.play(); 
    m_window.setMouseCursorVisible(false);
}

void Engine::DrawMainMenu(const sf::Event& event)
{
    while (true)
    {

    }
}

void Engine::DrawShop(const sf::Event& event)
{
    while (true)
    {

    }
}

void Engine::PoolEvent(const sf::Event& eventType)
{
    if (eventType.key.code == sf::Keyboard::Escape || eventType.type == sf::Event::Closed)
    {
        m_window.close();
    }
    if (eventType.key.code == sf::Keyboard::Tab)
    {
        DrawTab(m_event);
    }

}

void Engine::DrawMap(RayCastingProccessingForMapAndFrame* map)
{
    for (auto el : map->GetMap())
    {
        if (el->getPosition() == map->GetPlayerPositionFromMap())
        {
            m_window.draw(map->GetPlayerOnMap().m_playerShape);
        }
        else
        {
            m_window.draw(*el.get());
        }
    }
}
