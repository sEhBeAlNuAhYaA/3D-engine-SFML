#include "Includes.h"


int main(int argc, char* argv[])
{
    sf::RenderWindow mainWindow(sf::VideoMode(800, 600), "Game");
    sf::Event event;


    while (mainWindow.isOpen())
    {
        while (mainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }
        }
    }
}
