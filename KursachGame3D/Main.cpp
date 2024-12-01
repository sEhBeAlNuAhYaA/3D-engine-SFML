#include "Includes.h"
#include "Engine.h"

int main(int argc, char** argv)
{
    printf("START\n");
    switch (argc)
    {
    case 1:
    {
        Engine* engine = new Engine;
        delete engine;
        break;
    }
    case 3:
    {
        Engine* engine = new Engine(std::stoi(argv[1]), std::stoi(argv[2]));
        delete engine;
        break;
    }
    }    
    printf("END\n");
}
