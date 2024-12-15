#include "Includes.h"
#include "Engine.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    std::srand(std::time(0));
    
    Engine* engine = new Engine;
    delete engine;

    return 0;
}
