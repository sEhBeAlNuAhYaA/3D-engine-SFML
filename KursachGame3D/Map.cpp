#include "Map.h"

RayCastingProccessingForMapAndFrame::RayCastingProccessingForMapAndFrame(PlayerOnMap player)
{
    m_playerOnMap = player;
    FillShapesMap(player);
}

std::vector<std::shared_ptr<sf::Shape>> RayCastingProccessingForMapAndFrame::GetMap()
{
    return m_ShapesMap;
}

void RayCastingProccessingForMapAndFrame::setPlayerOnMapPostion(sf::Vector2f position)
{
    m_ShapesMap[m_playerOnMapPosition].get()->setPosition(position);
}

void RayCastingProccessingForMapAndFrame::FillShapesMap(PlayerOnMap player)
{
    sf::Image mapImage;
    mapImage.loadFromFile("map.png");

    for (int i = 0; i < mapImage.getSize().x; ++i)
    {

        for (int j = 0; j < mapImage.getSize().y; ++j)
        {
            if (mapImage.getPixel(j, i) != sf::Color::Red)
            {
                if (mapImage.getPixel(j, i) == sf::Color::White)
                {
                    /*
                    sf::RectangleShape rect(sf::Vector2f(MapObjectWidth, MapObjectWidth));
                    rect.setPosition(i * MapObjectWidth, j * MapObjectWidth);
                    m_ShapesMap.push_back(std::static_pointer_cast<sf::Shape>(std::make_shared<sf::RectangleShape>(rect)));
                    */
                    m_Map[j][i] = '1';
                }
                else if (mapImage.getPixel(j, i) == sf::Color::Blue)
                {
                    m_Map[j][i] = '2';
                }
                else 
                {
                    m_Map[j][i] = '0';
                }
            }
            else
            {
                player.m_playerShape.setPosition(i * MapObjectWidth, j * MapObjectWidth);
                m_playerOnMapPosition = m_ShapesMap.size();
                player.m_playerShape.setOrigin(sf::Vector2f(player.m_playerShape.getRadius(), player.m_playerShape.getRadius()));
                player.m_playerShape.setFillColor(sf::Color::Blue);
                m_ShapesMap.push_back(std::static_pointer_cast<sf::Shape>(std::make_shared<sf::CircleShape>(player.m_playerShape)));
                m_Map[j][i] = '@';
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
        //m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime, 0));
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
        //m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime, 0));
        m_playerOnMap.m_playerDirection = Right;
    }

}


void RayCastingProccessingForMapAndFrame::PlayerCollisionsWithMap(sf::Vector2f playerPosition, double deltaTime)
{
    switch (m_playerOnMap.m_playerDirection)
    {
    case Forward:
    {
        if (m_Map[(int)(playerPosition.y / (double)MapObjectWidth)][(int)(playerPosition.x / (double)MapObjectWidth)] != '0' && m_Map[(int)(playerPosition.y / (double)MapObjectWidth)][(int)(playerPosition.x / (double)MapObjectWidth)] != '@')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(-PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), -PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));
        }
        break;
    }
    case Backward:
    {
        if (m_Map[(int)(playerPosition.y / (double)MapObjectWidth)][(int)(playerPosition.x / (double)MapObjectWidth)] != '0' && m_Map[(int)(playerPosition.y / (double)MapObjectWidth)][(int)(playerPosition.x / (double)MapObjectWidth)] != '@')
        {
            m_ShapesMap[m_playerOnMapPosition].get()->move(sf::Vector2f(PLAYERSPEED * deltaTime * cos(m_playerOnMap.playerViewDirection), PLAYERSPEED * deltaTime * sin(m_playerOnMap.playerViewDirection)));

        }
        break;
    }
    }
}

void RayCastingProccessingForMapAndFrame::addMapBorder(DrawableEntityCollection& entitiesCollection)
{
    std::shared_ptr<sf::CircleShape> mapCircle = std::make_shared<sf::CircleShape>(DISTANCE);
    mapCircle->setPosition((float)DISTANCE, (float)DISTANCE);
    mapCircle->setOrigin((float)DISTANCE, (float)DISTANCE);
    mapCircle->setFillColor(sf::Color::Black);
    mapCircle->setOutlineThickness(5.f);
    mapCircle->setOutlineColor(sf::Color::White);

    std::shared_ptr<Entity> entity = std::make_shared<Entity>();
    entity->SetShape(mapCircle);
    //entitiesCollection.PushEntity(entity);
}

PlayerOnMap RayCastingProccessingForMapAndFrame::GetPlayerOnMap()
{
    return m_playerOnMap;
}

DrawableEntityCollection RayCastingProccessingForMapAndFrame::FillEntitiesCollectionForMapAndFrame(sf::RenderWindow* window)
{
    DrawableEntityCollection entityCollection;
    
    std::shared_ptr<sf::Texture> wallTexture = std::make_shared<sf::Texture>();
    wallTexture->loadFromFile("WallTexture.png");
    //float d = cos(m_playerOnMap.playerViewDirection) * 3.14;
    double currentViewAngle = m_playerOnMap.playerViewDirection - VIEWANGLE / 2.0;
    double lastHitx = 0;
    printf("=====================================================================\n");
    for (int lines = 0; lines < m_playerOnMap.m_FOV; ++lines)
    {
        //float a = cos(currentViewAngle) * 3.14;
     
        //printf("%f %f\n", m_playerOnMap.playerViewDirection, currentViewAngle);
        for (float distance = 0.f; distance <= DISTANCE; distance += 0.5f)
        {
            double x = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x + distance * cos(currentViewAngle);
            double y = m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y + distance * sin(currentViewAngle);
            if (m_Map[(int)(y / (double)MapObjectWidth)][(int)(x / (double)MapObjectWidth)] != '0' || (distance > DISTANCE - 1 && distance < DISTANCE))
            {
                //Rectangle for 3d
                //printf("cosinus - %f\n", floor(cos(currentViewAngle - m_playerOnMap.playerViewDirection) * 10) / 10);
                
                float lineL = sqrt(pow((x - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x), 2) + pow((y - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y), 2)) * cos(currentViewAngle - m_playerOnMap.playerViewDirection);
                //printf("lineL - %f; lineLF - %f;\n", sqrt(pow((x - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x), 2) + pow((y - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y), 2)), lineL);
                float planeAngle = (float)BASE_WIDTH / (2.0 * tan(VIEWANGLE / 2.0));
                //printf("planeAngle - %f\n", planeAngle);
                float wallHeight = MapObjectHeght * planeAngle / lineL;
                //3d frame rendering
                std::shared_ptr<sf::RectangleShape> rect3d = std::make_shared<sf::RectangleShape>(sf::Vector2f((float)BASE_WIDTH / m_playerOnMap.m_FOV, wallHeight));
                rect3d->setOrigin(0, wallHeight / 2.f);
                //rect3d->setOutlineThickness(1);
                //rect3d->setOutlineColor(sf::Color::Red);
                rect3d->setPosition(sf::Vector2f(lines * (float)BASE_WIDTH / m_playerOnMap.m_FOV, (float)BASE_HEIGHT / 2.0));
                int R = m_Map[(int)(y / (double)MapObjectWidth)][(int)(x / (double)MapObjectWidth)] == '2' ? 0 : 255;
                int G = m_Map[(int)(y / (double)MapObjectWidth)][(int)(x / (double)MapObjectWidth)] == '2' ? 0 : 255;
                int B = 255;
                int A = (distance > DISTANCE - 1 && distance < DISTANCE) ? 0 : 255 - (lineL * 255 / DISTANCE);
                rect3d->setFillColor(sf::Color(R, G, B, A));
                //entity creating
                std::shared_ptr<Entity> entity = std::make_shared<Entity>();
                entity->SetShape(rect3d);

                //texture extracting
                float i;
                double hitx = modf(x / MapObjectWidth, &i);
                double textureColumnWidth;
                if (abs(hitx - lastHitx) > 0.2)
                {
                    textureColumnWidth = hitx - 0;
                }
                else
                {
                    textureColumnWidth = (hitx - lastHitx);
                    //if (textureColumnWidth * 64 < 0)
                    {
                        //textureColumnWidth = 1 / 64;
                    }
                }
                
                sf::Vector2f ScaleForTexture(BASE_WIDTH / m_playerOnMap.m_FOV, wallHeight / 64.f);
                if (hitx < 0.015625)
                {
                    ScaleForTexture.x = hitx / 0.015625;
                }
                //printf("hitx = %f; B = %s\n", hitx, m_Map[(int)(y / (double)MapObjectWidth)][(int)(x / (double)MapObjectWidth)] == '2' ? "YES": "0");
                double hity = 0;
                if (lines != 0 && lines % m_playerOnMap.m_FOV / 2 == 0)
                {
                    //printf("size %f\n", lineL * (1 / tan(VIEWANGLE / 2) * 2) / m_playerOnMap.m_FOV * 10);
                }
                
                //printf("top %f; x%f; bottom %f\n", (float)VIEWANGLE / 2.f + VIEWANGLE / (double)m_playerOnMap.m_FOV + m_playerOnMap.m_playerDirection, currentViewAngle, (float)VIEWANGLE / 2.f - VIEWANGLE / (double)m_playerOnMap.m_FOV + m_playerOnMap.playerViewDirection);
                if(currentViewAngle >= (float)VIEWANGLE / 2.f - VIEWANGLE / (double)m_playerOnMap.m_FOV && currentViewAngle <= (float)VIEWANGLE / 2.f + VIEWANGLE / (double)m_playerOnMap.m_FOV)
                {
                    //float lineLL = sqrt(pow((x - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x), 2) + pow((y - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y), 2));
                    //float i;
                    //hitx = modf(x / MapObjectWidth, &i);
                    //printf("FOV = %f; lineL = %f; distance = %f; ViewAngle = %f\n",  lineLL * (1 / tan(VIEWANGLE / 2) * 2), lineLL, distance, currentViewAngle);
                }
                else
                {
                    //printf("top %f; x%f; bottom %f\n", (float)VIEWANGLE / 2.f + VIEWANGLE / (double)m_playerOnMap.m_FOV + m_playerOnMap.m_playerDirection, currentViewAngle, (float)VIEWANGLE / 2.f - VIEWANGLE / (double)m_playerOnMap.m_FOV + m_playerOnMap.playerViewDirection);
                    //printf("direction - %f\n", m_playerOnMap.playerViewDirection);
                  
                }
                
                entity->SetTexture(wallTexture, sf::IntRect(lastHitx * 64, 0, (textureColumnWidth * 64) < 1 ? 1 : (textureColumnWidth * 64), 64), ScaleForTexture);

                printf("hitx - %f, lastHitX - %f, textureW - %f, shapeX - %f;\n", hitx * 64, lastHitx * 64, (textureColumnWidth * 64), entity->GetShape()->getPosition().x);
                lastHitx = hitx;

                if (distance > DISTANCE - 1 && distance < DISTANCE)
                {
                    entity->GetSprite()->setColor(sf::Color::Black);
                }
                if ((int)(textureColumnWidth * 64) == 0)
                {
                    //entity->GetSprite()->setTextureRect(sf::IntRect(lastHitx * 64, 0, 1, 64));
                    //entity->GetSprite()->setColor(sf::Color::Red);
                }
                                    
                entityCollection.PushEntity(entity);

                //map lines
                float lineXMap = x - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().x + DISTANCE;
                float lineYMap = y - m_ShapesMap[m_playerOnMapPosition].get()->getPosition().y + DISTANCE;

                //map collection
                
                std::array<sf::Vertex, 2> line =
                {
                    sf::Vertex(m_playerOnMap.m_playerShape.getPosition(), sf::Color::White),
                    sf::Vertex(sf::Vector2f(lineXMap, lineYMap),sf::Color(255, 255, 255, 255 - (lineL * 255 / DISTANCE)))
                };
                entityCollection.mapDrawable.push_back(std::make_shared<std::array<sf::Vertex, 2>>(line));

                //map collisions
                std::shared_ptr<sf::RectangleShape> rectPixel = std::make_shared<sf::RectangleShape>(sf::Vector2f(2.f, 2.f));
                rectPixel->setPosition(lineXMap, lineYMap);
                std::shared_ptr<Entity> entityMapPixel = std::make_shared<Entity>();
                entityMapPixel->SetShape(rectPixel);
                entityCollection.PushEntityPixelForMap(entityMapPixel);


                    
                break;
            }
        }
        //sf::sleep(sf::milliseconds(1));
        //printf("+ - %f\n", floor(((double)VIEWANGLE / (double)m_playerOnMap.m_FOV) * 1000) / 1000);
        currentViewAngle += (double)VIEWANGLE / (double)m_playerOnMap.m_FOV;
    }
    addMapBorder(entityCollection);
    return entityCollection;
}



sf::Vector2f RayCastingProccessingForMapAndFrame::GetPlayerPositionFromMap()
{
    return m_ShapesMap[m_playerOnMapPosition].get()->getPosition();
}
