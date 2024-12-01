#include "Map.h"
#include <tgmath.h>

RayCastingProccessingForMapAndFrame::RayCastingProccessingForMapAndFrame(const PlayerOnMap& player)
    : m_textureLoader("textures.txt")
{
    m_playerOnMap = player;
    FillShapesMap();
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
                //else if (mapImage.getPixel(i, j) == sf::Color(100, 100, 100))
                //{
                    //m_Map[i][j] = 'e';
                //}
                else
                {
                    if (mapImage.getPixel(i, j) == sf::Color(100, 100, 100))
                    {
                        Entity entity(sf::Vector2f(i,j));
                        entity.SetImage("bobik.png");
                        m_EntityList.push_back(entity);
                        m_Map[i][j] = '0';
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

void RayCastingProccessingForMapAndFrame::PlayerMovement(double deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        m_playerOnMap.m_playerDirection = Forward;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_playerOnMap.playerViewDirection -= ROTATESPEED * deltaTime;
        m_playerOnMap.m_playerDirection = Left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        m_playerOnMap.m_playerDirection = Backward;
        PlayerCollisionsWithMap(m_ShapesMap[m_playerOnMapPosition]->getPosition(), deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_playerOnMap.playerViewDirection += ROTATESPEED * deltaTime;
        m_playerOnMap.m_playerDirection = Right;
    }
}

void RayCastingProccessingForMapAndFrame::PlayerCollisionsWithMap(const sf::Vector2f& playerPosition, double deltaTime)
{
    switch (m_playerOnMap.m_playerDirection)
    {
    case Forward:
    {
        if (m_Map[(int)(playerPosition.x / (double)MapObjectWidth)][(int)(playerPosition.y / (double)MapObjectWidth)] != '0' && m_Map[(int)(playerPosition.x / (double)MapObjectWidth)][(int)(playerPosition.y / (double)MapObjectWidth)] != '@')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        }
        break;
    }
    case Backward:
    {
        if (m_Map[(int)(playerPosition.x / (double)MapObjectWidth)][(int)(playerPosition.y / (double)MapObjectWidth)] != '0' && m_Map[(int)(playerPosition.x / (double)MapObjectWidth)][(int)(playerPosition.y / (double)MapObjectWidth)] != '@')
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
    armsTexture->loadFromFile("arms.png");
    std::shared_ptr<Drawable> drawableSprite = std::make_shared<Drawable>();
    drawableSprite->SetTextureAndSprite(
        armsTexture, 
        sf::IntRect(0, 0, armsTexture->getSize().x,
        armsTexture->getSize().y), 
        sf::Vector2f(1, 1), 
        sf::Vector2f(armsTexture->getSize().x / 2, armsTexture->getSize().y / 2),
        sf::Vector2f(sf::Vector2f(BASE_WIDTH / 2, BASE_HEIGHT - armsTexture->getSize().y / 2)));
    auto color = drawableSprite->GetSprite()->getColor();
    drawableSprite->GetSprite()->setColor(sf::Color(color.r, color.g - 50, color.b - 50, 255));
    entitiesCollection.PushDrawable(drawableSprite);
}

PlayerOnMap RayCastingProccessingForMapAndFrame::GetPlayerOnMap()
{
    return m_playerOnMap;
}

void RayCastingProccessingForMapAndFrame::fillEntityFromEngine(DrawableCollection& drawableCollection, std::map<double, sf::Vector2f> wallParams)
{
    const double x = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x;
    const double y = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y;

    for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
    {
        const double imageSizeX = it->m_Image.getSize().x;
        const double imageSizeY = it->m_Image.getSize().y;

        const double lineL = 16.0 * sqrt(pow((it->m_CurrentPosition.x - x / (double)MapObjectHeght), 2) + pow((it->m_CurrentPosition.y - y / (double)MapObjectWidth), 2));
        it->m_Distance = lineL;
    }

    std::sort(std::begin(m_EntityList), std::end(m_EntityList), [](const Entity& e1, const Entity& e2) {return e1.m_Distance > e2.m_Distance; });

    for (auto it = std::begin(m_EntityList), end = std::end(m_EntityList); it != end; ++it)
    {
        const double imageSizeX = it->m_Image.getSize().x;
        const double imageSizeY = it->m_Image.getSize().y;

        double spriteEntityDir = std::atan2(it->m_CurrentPosition.y - y / (double)MapObjectHeght, it->m_CurrentPosition.x - x / (double)MapObjectWidth);

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
    return drawableCollection;
}

sf::Vector2f RayCastingProccessingForMapAndFrame::GetPlayerPositionFromMap()
{
    return m_ShapesMap[m_playerOnMapPosition].get()->getPosition();
}
