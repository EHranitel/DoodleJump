#include "Storage.h"
#include "Logics.h"
#include "Graphics.h"

enum GameState
{
    NotStarted,
    Started,
    Finished,
};
 
int main()
{
    List<GameObject*> storage;
    GraphicsManager graphics;
    LogicManager logics;

    graphics.changeFont("TimesNewRoman.ttf");

    Character doodle;
    doodle.r.x = 500;
    doodle.r.y = 730;
    doodle.accelerationY = 0.25;
   
    storage.append(&doodle);

    storage[0]->type = ObjectType::OCharacter;

    Platform platforms[25];

    platforms[0].r.x = 500;
    platforms[0].r.y = 770;

    storage.append(&platforms[0]);

    storage[1]->type = ObjectType::OPlatform;

    srand(time(0));

    for(int i = 1; i < 25; i++)
    {   
        platforms[i].r.x = platforms[i - 1].r.x - 150  + rand() % 300;

        if (storage[i]->r.x >= 1000)
            {
                storage[i]->r.x -= 200;
            }

        else if (storage[i]->r.x <= 0)
            {
                storage[i]->r.x += 200;
            }

        platforms[i].r.y = platforms[i - 1].r.y - 200  + rand() % 100;

        storage.append(&platforms[i]);

        storage[i + 1]->type = ObjectType::OPlatform;
    }

    Enemy monster;

    monster.r.x = 500;
    monster.r.y = 90;
    
    monster.speedX = -5;

    storage.append(&monster);

    storage[26]->type = ObjectType::OEnemy;

    float dT = 0.1;
    int checkedPlat;

    GameState state = GameState::NotStarted;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    graphics.updateGraphics(logics.getScore(), storage, dT, &window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            else if(event.type == sf::Event::KeyPressed && state == GameState::NotStarted) 
            {
                state = GameState::Started;
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                logics.shoot(storage);
            }
        }

        if (state == GameState::NotStarted)
        {
            continue;
        }

        logics.controlCharacter(storage);

        logics.collide(storage);

        logics.stopScreenMovement(storage);

        logics.moveAll(storage, dT);

        logics.updateScore(storage, dT);

        graphics.updateGraphics(logics.getScore(), storage, dT, &window);

        if (logics.checkLoseConditions(storage))
        {
            return 0;
        }
    }

    return 0;
}