#include "Engine.h"

Engine::Engine()
    : m_window(
        sf::VideoMode(BASE_WIDTH, BASE_HEIGHT), 
        "Game", 
        sf::Style::Default, 
        sf::ContextSettings(0U, 0U, 8, 0U, 1U, 0U, false))
{
    printf("ENGINE CONSTRUCTOR - FRAME LIMIT 60 - RESOLUTION %dx%d\n", BASE_WIDTH, BASE_HEIGHT);
    m_RayCastingProccessingForMapAndFrame = new RayCastingProccessingForMapAndFrame(PlayerOnMap(512));
    m_window.setFramerateLimit(60);
    DoRenderMainWindow();
}

Engine::Engine(const int framerate, const int fov)
    : m_window(
        sf::VideoMode(BASE_WIDTH, BASE_HEIGHT), 
        "Game", 
        sf::Style::Default, 
        sf::ContextSettings(0U, 0U, 8, 0U, 1U, 0U, false))
{
    m_window.setMouseCursorVisible(false);
    printf("ENGINE CONSTRUCTOR - FRAME LIMIT %d - RESOLUTION %dx%d\n",framerate, BASE_WIDTH, BASE_HEIGHT);
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
    Update();
}

void Engine::Update()
{
    sf::Clock GlobalClock;
    while (m_window.isOpen())
    {
        PoolEvent();
        m_window.clear();
        if (m_window.hasFocus())
        {
            m_RayCastingProccessingForMapAndFrame->PlayerMovement(GlobalClock.getElapsedTime().asSeconds());
        }
        GlobalClock.restart();
        
        DrawableCollection frame = m_RayCastingProccessingForMapAndFrame->FillEntitiesCollectionForMapAndFrame();
        //drawing frame
        DrawEntities(frame);
        DrawMap(*m_RayCastingProccessingForMapAndFrame);
        m_window.display();
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
}

void Engine::PoolEvent()
{
    while (m_window.pollEvent(m_event))
    {
        if (m_event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        if (m_event.type == sf::Event::Resized)
        {
            double scale = (double)m_event.size.width / (double)m_event.size.height;
            double newWidth;
            double newHeight;

            if (scale > (double)BASE_WIDTH / (double)BASE_HEIGHT)
            {
                newWidth = (double)BASE_HEIGHT * scale;
                newHeight = (double)BASE_HEIGHT;
            }
            else
            {
                newWidth = (double)BASE_WIDTH;
                newHeight = (double)BASE_WIDTH / scale;
            }

            double offsetX = (double)BASE_WIDTH / 2 - (double)newWidth / 2;
            double offsetY = (double)BASE_HEIGHT / 2 - (double)newHeight / 2;

            m_window.setView(sf::View(sf::FloatRect(offsetX, offsetY, newWidth, newHeight)));
        }
    }
}

void Engine::DrawMap(RayCastingProccessingForMapAndFrame map)
{
    for (auto el : map.GetMap())
    {
        if (el->getPosition() == map.GetPlayerPositionFromMap())
        {
            m_window.draw(map.GetPlayerOnMap().m_playerShape);
        }
        else
        {
            m_window.draw(*el.get());
        }
    }
}
