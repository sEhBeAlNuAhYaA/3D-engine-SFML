#include "Map.h"

RayCastingProccessingForMapAndFrame::RayCastingProccessingForMapAndFrame(const PlayerOnMap& player)
    : m_textureLoader("textures.txt")
{
    m_hitSound.openFromFile("hitSound.mp3");
    m_hitSound.setVolume(5);
    m_playerHitSound.openFromFile("playerHit.mp3");
    m_playerHitSound.setVolume(15);
    m_playerHitSound.pause();
    m_playerOnMap = player;
    FillShapesMap();
    FindPathForEveryEntity();
}

std::vector<std::shared_ptr<sf::Shape>> RayCastingProccessingForMapAndFrame::GetMap()
{
    return m_ShapesMap;
}

void RayCastingProccessingForMapAndFrame::setPlayerOnMapPostion(const sf::Vector2f& position)
{
    m_ShapesMap[m_playerOnMapPosition].get()->setPosition(position);
}

void RayCastingProccessingForMapAndFrame::FillShapesMap()
{
    sf::Image mapImage;
    mapImage.loadFromFile("map.png");

    for (int i = 0; i < mapImage.getSize().x; ++i)
    {

        for (int j = 0; j < mapImage.getSize().y; ++j)
        {
            if (mapImage.getPixel(i, j) != sf::Color::Red)
            {
                if (mapImage.getPixel(i, j) == sf::Color::White)
                {
                    m_Map[i][j] = '1';
                }
                else if (mapImage.getPixel(i, j) == sf::Color::Blue)
                {
                    m_Map[i][j] = '2';
                }
                else if (mapImage.getPixel(i, j) == sf::Color::Green)
                {
                    m_Map[i][j] = '3';
                }

                else
                {
                    if (mapImage.getPixel(i, j) == sf::Color(100, 100, 100))
                    {
                        Entity entity(sf::Vector2f(i,j), 100);
                        entity.SetImage("bobik.png");
                        m_EntityList.push_back(entity);
                        m_Map[i][j] = 'e';
                        continue;
                    }
                    m_Map[i][j] = '0';
                }
            }
            else
            {
                m_playerOnMapPosition = m_ShapesMap.size();
                m_playerOnMap.m_playerShape.setOrigin(sf::Vector2f(m_playerOnMap.m_playerShape.getRadius(), m_playerOnMap.m_playerShape.getRadius()));
                m_playerOnMap.m_playerShape.setFillColor(sf::Color::White);
                m_ShapesMap.push_back(std::static_pointer_cast<sf::Shape>(std::make_shared<sf::CircleShape>(m_playerOnMap.m_playerShape)));
                m_ShapesMap[m_playerOnMapPosition]->setPosition(sf::Vector2f(i * MapObjectWidth, j * MapObjectHeght));
                m_Map[i][j] = '@';
            }
        }
    }
}

bool RayCastingProccessingForMapAndFrame::isValidPosition(int x, int y, std::vector<std::vector<bool>>& visited)
{
    return (x >= 0 && x < 50 && y >= 0 && y < 50 && m_Map[x][y] != '1' && m_Map[x][y] != '2' && m_Map[x][y] != '3' && !visited[x][y]);
}

void RayCastingProccessingForMapAndFrame::PlayerMovement(sf::Clock& globalClock, sf::Event& event, sf::Clock& playerHitClock, sf::RenderWindow& m_window)
{
    if (sf::Mouse::getPosition(m_window).x != BASE_WIDTH / 2)
    {
        //printf("MouseDifX:%f\n", (sf::Mouse::getPosition(m_window).x - (BASE_WIDTH / 2)) / 10.f);
        m_playerOnMap.playerViewDirection += (sf::Mouse::getPosition(m_window).x - (BASE_WIDTH / 2)) / 1000.f;
        sf::Mouse::setPosition(sf::Vector2i(BASE_WIDTH / 2, BASE_HEIGHT / 2), m_window);
    }
    auto deltaTime = globalClock.getElapsedTime().asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        m_playerOnMap.m_playerDirection = Direction::Forward;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection - 1.57), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection - 1.57)));
        m_playerOnMap.m_playerDirection = Direction::Left;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        m_playerOnMap.m_playerDirection = Direction::Backward;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection - 1.57),-PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection - 1.57)));
        m_playerOnMap.m_playerDirection = Direction::Right;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        DoHit(playerHitClock);
    }
    else
    {
        if (playerHitClock.getElapsedTime().asSeconds() > 0.01) m_playerOnMap.m_bisHiting = false;
    }
    globalClock.restart();
}

void RayCastingProccessingForMapAndFrame::PlayerCollisionsWithMap(const sf::Vector2f& playerPosition, double deltaTime)
{
    auto point = m_Map[(int)(playerPosition.x / (double)MapObjectWidth)][(int)(playerPosition.y / (double)MapObjectWidth)];
    switch (m_playerOnMap.m_playerDirection)
    {
    case Direction::Forward:
    {
        if (point != '0' && point != '@' && point != 'e')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        }
        break;
    }
    case Direction::Left:
    {
        if (point != '0' && point != '@' && point != 'e') 
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection - 1.57), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection - 1.57)));
        }
        break;
    }
    case Direction::Right:
    {
        if (point != '0' && point != '@' && point != 'e')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection - 1.57), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection - 1.57)));
        }
        break;
    }
    case Direction::Backward:
    {
        if (point != '0' && point != '@' && point != 'e')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        }
        break;
    }
    }
}

void RayCastingProccessingForMapAndFrame::addMapBorder(DrawableCollection& entitiesCollection)
{
    std::shared_ptr<sf::CircleShape> mapCircle = std::make_shared<sf::CircleShape>(DISTANCE);
    mapCircle->setPosition((float)DISTANCE + 5.f, (float)DISTANCE + 5.f);
    mapCircle->setOrigin((float)DISTANCE, (float)DISTANCE);
    mapCircle->setFillColor(sf::Color::Black);
    mapCircle->setOutlineThickness(5.f);
    mapCircle->setOutlineColor(sf::Color::White);
    std::shared_ptr<Drawable> drawableShape = std::make_shared<Drawable>();
    drawableShape->SetShape(mapCircle);
    entitiesCollection.PushDrawable(drawableShape);
}

void RayCastingProccessingForMapAndFrame::addBackGroundShape(DrawableCollection& entitiesCollection)
{
    std::shared_ptr<sf::RectangleShape> background = std::make_shared<sf::RectangleShape>(sf::Vector2f((float)BASE_WIDTH, (float)BASE_HEIGHT));
    background->setFillColor(sf::Color(20, 0, 0));
    std::shared_ptr<Drawable> drawableShape = std::make_shared<Drawable>();
    drawableShape->SetShape(background);
    entitiesCollection.PushDrawable(drawableShape);
}

void RayCastingProccessingForMapAndFrame::addArms(DrawableCollection& entitiesCollection)
{
    std::shared_ptr<sf::Texture> armsTexture = std::make_shared<sf::Texture>();
    armsTexture = m_textureLoader.GetSpriteTexture(m_playerOnMap.m_bisHiting ? '8' : '9');
    std::shared_ptr<Drawable> drawableSprite = std::make_shared<Drawable>();
    auto scale = sf::Vector2f(1,1);
    if (m_playerOnMap.m_gun == Gun::Pistol)
    {
        scale = sf::Vector2f(0.5, 0.5);
    }
    if (m_playerOnMap.m_gun == Gun::Rifle)
    {
        scale = sf::Vector2f(2, 2);
    }
    drawableSprite->SetTextureAndSprite(
        armsTexture, 
        sf::IntRect(0, 0, armsTexture->getSize().x,
        armsTexture->getSize().y), 
        scale, 
        sf::Vector2f(armsTexture->getSize().x / 2, armsTexture->getSize().y / 2),
        sf::Vector2f(sf::Vector2f(BASE_WIDTH / 2, BASE_HEIGHT - armsTexture->getSize().y / 2)));
    auto color = drawableSprite->GetSprite()->getColor();
    drawableSprite->GetSprite()->setColor(sf::Color(color.r, color.g - 50, color.b - 50, 255));
    entitiesCollection.PushDrawable(drawableSprite);
}

void RayCastingProccessingForMapAndFrame::addHud(DrawableCollection& entitiesCollection)
{
    std::shared_ptr<sf::RectangleShape> HP = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_playerOnMap.m_HP * 2, 10));
    HP->setPosition(sf::Vector2f(25, BASE_HEIGHT - 40));
    HP->setFillColor(sf::Color::Red);
    std::shared_ptr<sf::RectangleShape> HPborder = std::make_shared<sf::RectangleShape>(sf::Vector2f(200, 10));
    HPborder->setPosition(sf::Vector2f(25, BASE_HEIGHT - 40));
    HPborder->setFillColor(sf::Color::Transparent);
    HPborder->setOutlineThickness(2);
    HPborder->setOutlineColor(sf::Color::White);
    std::shared_ptr<Drawable> drawableShape = std::make_shared<Drawable>();
    drawableShape->SetShape(HP);
    std::shared_ptr<Drawable> drawableShape1 = std::make_shared<Drawable>();
    drawableShape1->SetShape(HPborder);
    entitiesCollection.PushDrawable(drawableShape);
    entitiesCollection.PushDrawable(drawableShape1);

    std::shared_ptr<sf::RectangleShape> Armor = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_playerOnMap.m_Armor * 2, 10));
    Armor->setPosition(sf::Vector2f(25, BASE_HEIGHT - 70));
    Armor->setFillColor(sf::Color::Blue);
    std::shared_ptr<sf::RectangleShape> Armorborder = std::make_shared<sf::RectangleShape>(sf::Vector2f(200, 10));
    Armorborder->setPosition(sf::Vector2f(25, BASE_HEIGHT - 70));
    Armorborder->setFillColor(sf::Color::Transparent);
    Armorborder->setOutlineThickness(2);
    Armorborder->setOutlineColor(sf::Color::White);
    std::shared_ptr<Drawable> drawableShape2 = std::make_shared<Drawable>();
    drawableShape2->SetShape(Armor);
    std::shared_ptr<Drawable> drawableShape3 = std::make_shared<Drawable>();
    drawableShape3->SetShape(Armorborder);
    entitiesCollection.PushDrawable(drawableShape2);
    entitiesCollection.PushDrawable(drawableShape3);
}

PlayerOnMap& RayCastingProccessingForMapAndFrame::GetPlayerOnMap()
{
    return m_playerOnMap;
}

void RayCastingProccessingForMapAndFrame::DoHit(sf::Clock& clock)
{
    m_playerOnMap.updateGun();
    float time = 0;
    if (m_playerOnMap.m_gun == Gun::Pistol)
    {
        time = 0.25;
    }
    if (m_playerOnMap.m_gun == Gun::Shootgun)
    {
        time = 0.5;
    }
    if (m_playerOnMap.m_gun == Gun::Rifle)
    {
        time = 1;
    }
    if (clock.getElapsedTime().asSeconds() > time)
    {
        m_hitSound.play();
        m_playerOnMap.m_bisHiting = true;
        std::vector<int> toDelete;
        const double x = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x;
        const double y = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y;

        for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
        {
            if (it->m_Distance <= MapObjectWidth * 5)
            {
                float CurrentPositionX = it->m_CurrentPosition.x;
                float CurrentPositionY = it->m_CurrentPosition.y;

                double spriteEntityDir = std::atan2(CurrentPositionY - y / (double)MapObjectHeght, CurrentPositionX - x / (double)MapObjectWidth);

                const double planeAngle = (double)BASE_WIDTH / (2.0 * tan(VIEWANGLE / 2.0));
                const double wallHeight = MapObjectHeght * planeAngle / it->m_Distance;

                //intervals issue
                auto leftCenter = m_playerOnMap.playerViewDirection;
                auto rightCenter = m_playerOnMap.playerViewDirection;
                auto left = m_playerOnMap.playerViewDirection - VIEWANGLE / 2.0;
                auto right = m_playerOnMap.playerViewDirection + VIEWANGLE / 2.0;

                if (left < -3.14)
                {
                    leftCenter = 3.14;
                    rightCenter = -3.14;
                    left += 6.283;
                }
                if (right > 3.14)
                {
                    rightCenter = -3.14;
                    leftCenter = 3.14;
                    right -= 6.283;
                }

                auto spritedif = (spriteEntityDir - left);
                //spritedif -> [0;3.14]<-|->[3.14;4]
                spritedif = (spritedif) / VIEWANGLE;

                if ((spriteEntityDir >= left && spriteEntityDir < leftCenter) || (spriteEntityDir <= right && spriteEntityDir > rightCenter))
                {
                    m_EntityList[std::distance(std::begin(m_EntityList), it)].m_HP -= m_playerOnMap.m_damage;
                    if (it->m_HP <= 0)
                    {
                        m_playerOnMap.m_killsCounter.y += 1;
                        toDelete.push_back(std::distance(std::begin(m_EntityList), it));
                    }
                    //printf("N:%d, HP:%f\n", std::distance(std::begin(m_EntityList), it), it->m_HP);
                }
            }
        }

        for (int i = toDelete.size() - 1; i >= 0; --i)
        {
            m_EntityList.erase(std::begin(m_EntityList) + toDelete[i], std::begin(m_EntityList) + toDelete[i] + 1);
        }
        clock.restart();
    }
}

void RayCastingProccessingForMapAndFrame::checkForKills()
{
    if (m_playerOnMap.m_killsCounter.x != m_playerOnMap.m_killsCounter.y)
    {
        m_playerOnMap.m_score = m_playerOnMap.m_killsCounter.y * 150;
        switch (m_playerOnMap.m_killsCounter.y)
        {
        case 1: {
            m_killSound.openFromFile("first.mp3");
            break;
        }
        case 2: {
            m_killSound.openFromFile("second.mp3");
            break;
        }
        case 3: {
            m_killSound.openFromFile("third.mp3");
            break;
        }
        case 4: {
            m_killSound.openFromFile("forth.mp3");
            break;
        }
        case 5: {
            m_killSound.openFromFile("fifth.mp3");
            break;
        }
        default:
        {
            m_killSound.openFromFile("rampage.mp3");
            break;
        }
        }
        m_killSound.setVolume(5);
        m_killSound.play();
    }
    m_playerOnMap.m_killsCounter.x = m_playerOnMap.m_killsCounter.y;
}

void RayCastingProccessingForMapAndFrame::playerDamage(sf::Clock& clockForEntity)
{
    for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
    {
        if (it->m_Distance <= 20 && it->m_Distance > 0)
        {
            if (clockForEntity.getElapsedTime().asSeconds() > 1)
            {
                m_playerHitSound.play();
                if (m_playerOnMap.m_Armor - m_playerOnMap.m_entityDamage > 0)
                {
                    m_playerOnMap.m_Armor -= m_playerOnMap.m_entityDamage;
                }
                else
                {
                    m_playerOnMap.m_HP -= (m_playerOnMap.m_entityDamage - m_playerOnMap.m_Armor);
                    if (m_playerOnMap.m_HP < 0) m_playerOnMap.m_HP = 0;
                    m_playerOnMap.m_Armor = 0;
                    
                }
                clockForEntity.restart();
            }
        }
    }
}

bool RayCastingProccessingForMapAndFrame::IsPlayerDead()
{
    return m_playerOnMap.m_HP <= 0;
}

void RayCastingProccessingForMapAndFrame::fillEntityFromEngine(DrawableCollection& drawableCollection, std::map<double, sf::Vector2f> wallParams)
{
    const double x = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x;
    const double y = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y;

    float CurrentPositionX;
    float CurrentPositionY;
    float a;

    for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
    {
        const double imageSizeX = it->m_Image.getSize().x;
        const double imageSizeY = it->m_Image.getSize().y;

        CurrentPositionX = it->m_CurrentPosition.x;
        CurrentPositionY = it->m_CurrentPosition.y;

        const double lineL = 16.0 * sqrt(pow((CurrentPositionX - x / (double)MapObjectHeght), 2) + pow((CurrentPositionY - y / (double)MapObjectWidth), 2));
        it->m_Distance = lineL;
    }

    std::sort(std::begin(m_EntityList), std::end(m_EntityList), [](const Entity& e1, const Entity& e2) {return e1.m_Distance > e2.m_Distance; });

    for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
    {
        CurrentPositionX = it->m_CurrentPosition.x;
        CurrentPositionY = it->m_CurrentPosition.y;

        if (modf(CurrentPositionX, &a) != 0 || modf(CurrentPositionY, &a) != 0)
        {
            if (modf(CurrentPositionX, &a) == 0)
            {
                CurrentPositionX += 0.5;
            }

            if (modf(CurrentPositionY, &a) == 0)
            {
                CurrentPositionY += 0.5;
            }
        }
        
        const double imageSizeX = it->m_Image.getSize().x;
        const double imageSizeY = it->m_Image.getSize().y;

        double spriteEntityDir = std::atan2(CurrentPositionY - y / (double)MapObjectHeght, CurrentPositionX - x / (double)MapObjectWidth);

        const double planeAngle = (double)BASE_WIDTH / (2.0 * tan(VIEWANGLE / 2.0));
        const double wallHeight = MapObjectHeght * planeAngle / it->m_Distance;

        //intervals issue
        auto leftCenter = m_playerOnMap.playerViewDirection;
        auto rightCenter = m_playerOnMap.playerViewDirection;
        auto left = m_playerOnMap.playerViewDirection - VIEWANGLE / 2.0;
        auto right = m_playerOnMap.playerViewDirection + VIEWANGLE / 2.0;

        if (left < -3.14)
        {
            leftCenter = 3.14;
            rightCenter = -3.14;
            left += 6.283;
        }
        if (right > 3.14)
        {
            rightCenter = -3.14;
            leftCenter = 3.14;
            right -= 6.283;
        }

        auto spritedif = (spriteEntityDir - left);
        //spritedif -> [0;3.14]<-|->[3.14;4]
        spritedif = (spritedif) / VIEWANGLE;

        if ((spriteEntityDir >= left && spriteEntityDir < leftCenter ) || (spriteEntityDir <= right && spriteEntityDir > rightCenter))
        {
            if (spritedif < 0)
            {
                //spritedif -> [3.14;4] -> [0;1]
                spritedif = 1.f - (((abs(spritedif) - 3.0)) / (4 - 3.0));
            }

            it->entityCollisionsAndLoadingTexture(wallParams, 
                sf::Vector2f(imageSizeX / 2, imageSizeY / 2 * 0.5),
                sf::Vector2f((wallHeight / imageSizeY), wallHeight / imageSizeY),
                sf::Vector2f((int)(spritedif * (double)BASE_WIDTH), (int)((double)BASE_HEIGHT / 2.0)));

            drawableCollection.PushDrawable(it->m_DrawableSprite);
        }
    }
}

void RayCastingProccessingForMapAndFrame::FindPathForEveryEntity()
{
    for (int i = 0; i < m_EntityList.size(); ++i)
    {
        m_EntityList[i].m_pathToPlayer = FindShortestPath(m_EntityList[i].m_CurrentPosition, m_ShapesMap[m_playerOnMapPosition]->getPosition());
    }
}

std::vector<sf::Vector2i> RayCastingProccessingForMapAndFrame::FindShortestPath(const sf::Vector2f& entityPosition, const sf::Vector2f& playerPosition)
{
    std::vector<std::vector<bool>> visited(50, std::vector<bool>(50, false));
    std::vector<sf::Vector2i> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    std::queue<std::pair<sf::Vector2i, std::vector<sf::Vector2i>>> q;
    q.push({ (sf::Vector2i)entityPosition, {(sf::Vector2i)entityPosition} });
    visited[(int)entityPosition.x / 16][(int)entityPosition.y / 16] = true;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        sf::Vector2i point = (sf::Vector2i)current.first;
        std::vector<sf::Vector2i> path = current.second;

        if (point.x == (int)playerPosition.x / 16 && point.y == (int)playerPosition.y / 16) {
            return path;
        }

        for (const auto& dir : directions) {
            int newX = point.x + dir.x;
            int newY = point.y + dir.y;

            if (isValidPosition(newX, newY, visited)) {
                visited[newX][newY] = true;
                std::vector<sf::Vector2i> newPath = path;
                newPath.push_back({ newX, newY });
                q.push({ {newX, newY}, newPath });
            }
        }
    }
    return {};
}

void RayCastingProccessingForMapAndFrame::MoveEntityToPlayer(sf::Clock& clock, double deltaTime)
{

    for (int i = 0; i < m_EntityList.size(); ++i)
    {
        if (m_EntityList[i].m_Distance <= 16 * 7 && m_EntityList[i].m_pathToPlayer.size() < 10)
        {
            if (m_EntityList[i].m_pathToPlayer.size() == 1)
            {
                clock.restart();
                continue;
            }
            float distanceX = m_EntityList[i].m_CurrentPosition.x - m_EntityList[i].m_pathToPlayer[1].x;
            float distanceY = m_EntityList[i].m_CurrentPosition.y - m_EntityList[i].m_pathToPlayer[1].y;

            float interpolationPos = 0;

            if (distanceX > 0)
            {
                m_EntityList[i].m_CurrentPosition.x = m_EntityList[i].m_CurrentPosition.x - deltaTime * BOTSPEED;
                if (m_EntityList[i].m_CurrentPosition.x + 0.1 > m_EntityList[i].m_pathToPlayer[1].x && m_EntityList[i].m_CurrentPosition.x - 0.1 < m_EntityList[i].m_pathToPlayer[1].x)
                {
                    m_EntityList[i].m_CurrentPosition.x = m_EntityList[i].m_pathToPlayer[1].x;
                }
                clock.restart();
                continue;
            }
            else if (distanceX < 0)
            {
                m_EntityList[i].m_CurrentPosition.x = m_EntityList[i].m_CurrentPosition.x + deltaTime * BOTSPEED;
                if (m_EntityList[i].m_CurrentPosition.x + 0.1 > m_EntityList[i].m_pathToPlayer[1].x && m_EntityList[i].m_CurrentPosition.x - 0.1 < m_EntityList[i].m_pathToPlayer[1].x)
                {
                    m_EntityList[i].m_CurrentPosition.x = m_EntityList[i].m_pathToPlayer[1].x;
                }
            }
            else if (distanceX == 0)
            {
                interpolationPos = 0;

                if (distanceY > 0)
                {
                    m_EntityList[i].m_CurrentPosition.y = m_EntityList[i].m_CurrentPosition.y - deltaTime * BOTSPEED;
                }
                else if (distanceY < 0)
                {
                    m_EntityList[i].m_CurrentPosition.y = m_EntityList[i].m_CurrentPosition.y + deltaTime * BOTSPEED;
                }

                if (m_EntityList[i].m_CurrentPosition.y + 0.1 > m_EntityList[i].m_pathToPlayer[1].y && m_EntityList[i].m_CurrentPosition.y - 0.1 < m_EntityList[i].m_pathToPlayer[1].y)
                {
                   m_EntityList[i].m_CurrentPosition.y = m_EntityList[i].m_pathToPlayer[1].y;
                }
                clock.restart();
                continue;
            }
        }
        clock.restart();
    }
}

bool RayCastingProccessingForMapAndFrame::isLvlDone()
{
    return m_EntityList.size() == 0;
}

DrawableCollection RayCastingProccessingForMapAndFrame::FillEntitiesCollectionForMapAndFrame()
{
    DrawableCollection drawableCollection;
    addBackGroundShape(drawableCollection);

    std::shared_ptr<sf::Texture> wallTexture = std::make_shared<sf::Texture>();
    std::map<double, sf::Vector2f> distances;

    double currentViewAngle = m_playerOnMap.playerViewDirection - VIEWANGLE / 2.0;
    for (int lines = 0; lines < m_playerOnMap.m_FOV; ++lines)
    {
        for (double distance = 0.f; distance <= DISTANCE; distance += 0.5f)
        {
            const double x = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x + distance * cos(currentViewAngle);
            const double y = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y + distance * sin(currentViewAngle);
            const double yPosOnMap = y / (double)MapObjectHeght;
            const double xPosOnMap = x / (double)MapObjectWidth;
            auto currentCellPoint = m_Map[(int)xPosOnMap][(int)yPosOnMap];
            if ((currentCellPoint != '0' && currentCellPoint != '@' && currentCellPoint != 'e') || (distance > DISTANCE - 1 && distance < DISTANCE))
            {
                const double lineL = sqrt(pow((x - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x), 2) + pow((y - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y), 2)) * cos(currentViewAngle - m_playerOnMap.playerViewDirection);
                const double planeAngle = (double)BASE_WIDTH / (2.0 * tan(VIEWANGLE / 2.0));
                const double wallHeight = MapObjectHeght * planeAngle / lineL;

                if ((currentCellPoint != '0' && currentCellPoint != '@' && currentCellPoint != 'e'))
                {
                    float i;
                    double hit = 0.005;
                    if (abs(xPosOnMap - round(xPosOnMap)) <= abs(yPosOnMap - round(yPosOnMap)))
                    {
                        hit = modf(yPosOnMap, &i);
                    }
                    else
                    {
                        hit = modf(xPosOnMap, &i);
                    }

                    wallTexture = m_textureLoader.GetTextureByMap(currentCellPoint);
                    const auto wallTextureWidth = wallTexture->getSize().x;
                    const auto wallTextureHeight = wallTexture->getSize().y;

                    sf::Vector2f ScaleForTexture(BASE_WIDTH / m_playerOnMap.m_FOV, wallHeight / wallTextureHeight);

                    const auto wallPosition = sf::Vector2f(lines * (double)BASE_WIDTH / m_playerOnMap.m_FOV, (double)BASE_HEIGHT / 2.0);

                    std::shared_ptr<Drawable> drawableSprite = std::make_shared<Drawable>();
                    drawableSprite->SetTextureAndSprite(
                        wallTexture,
                        sf::IntRect(hit * wallTextureWidth, 0, 1, wallTexture->getSize().y),
                        ScaleForTexture,
                        sf::Vector2f(0, wallTexture->getSize().y / 2),
                        wallPosition);
                    drawableSprite->GetSprite()->setColor(Tools::SetTransparetForColor(drawableSprite->GetSprite()->getColor(), 255 - lineL * 255 / DISTANCE));
                    if (distance > DISTANCE - 1 && distance < DISTANCE)
                    {
                        drawableSprite->GetSprite()->setColor(Tools::SetTransparetForColor(drawableSprite->GetSprite()->getColor(), 0));
                    }
                    drawableCollection.PushDrawable(drawableSprite);
                    distances.insert(std::pair<double, sf::Vector2f>(lineL, sf::Vector2f(lines * (double)BASE_WIDTH / m_playerOnMap.m_FOV, (double)BASE_HEIGHT / 2.0)));
                }

                //map lines
                float lineXMap = x - GetPlayerPositionFromMap().x + m_playerOnMap.m_playerShape.getPosition().x;
                float lineYMap = y - GetPlayerPositionFromMap().y + m_playerOnMap.m_playerShape.getPosition().y;

                //map collection
                std::array<sf::Vertex, 2> line =
                {
                    sf::Vertex(m_playerOnMap.m_playerShape.getPosition(), sf::Color::White),
                    sf::Vertex(sf::Vector2f(lineXMap, lineYMap),sf::Color(255, 255, 255, 255 - (lineL * 255 / DISTANCE)))
                };
                drawableCollection.mapDrawable.push_back(std::make_shared<std::array<sf::Vertex, 2>>(line));

                break;
            }
           
        }
        currentViewAngle += (double)VIEWANGLE / (double)m_playerOnMap.m_FOV;
        if (m_playerOnMap.playerViewDirection > 3.14) m_playerOnMap.playerViewDirection = -3.14;
        if (m_playerOnMap.playerViewDirection < -3.14) m_playerOnMap.playerViewDirection = 3.14;
    }
    fillEntityFromEngine(drawableCollection, distances);
    addMapBorder(drawableCollection);
    addArms(drawableCollection);
    addHud(drawableCollection);
    FindPathForEveryEntity();

    return drawableCollection;
}

sf::Vector2f RayCastingProccessingForMapAndFrame::GetPlayerPositionFromMap()
{
    return m_ShapesMap[m_playerOnMapPosition].get()->getPosition();
}
