#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "Vector2f.h"
#include "List.h"

enum GameState
{
    NotStarted,
    Started,
    Finished,
};

enum ObjectType
{
    OCharacter,
    OEnemy,
    OPlatform,
    OProjectile,
};

class GameObject
{
    public:

        Vector2f r = Vector2f(1, 0);
        float speedX = 0;
        float speedY = 0;
        float keptSpeedY = 0;
        float accelerationX = 0;
        float accelerationY = 0;

        int orientaion;

        ObjectType type;
        
        virtual void draw(sf::RenderWindow* window)
        {
            std::cout << "test" << std::endl;
        }

        virtual void move(float dT)
        {}
};

class Character : public GameObject
{
    public:

        ObjectType type = ObjectType::OCharacter;

        float keptSpeedY = 0;

        int orientaion = 1;

        void draw(sf::RenderWindow* window)
        {
            sf::RectangleShape body;
            body.setSize(sf::Vector2f(60, 60));
            body.setFillColor(sf::Color::Green);
            body.setPosition(r.x - 30, r.y - 60);

            sf::RectangleShape weapon;
            weapon.setSize(sf::Vector2f(30, 10));
            weapon.setFillColor(sf::Color::Green);
            weapon.setPosition(r.x + orientaion * 30, r.y - 35);

            window->draw(body);
            window->draw(weapon);
        }

        void move(float dT)
        {
            r.x = r.x + speedX * dT + accelerationX * dT * dT / 2;
            r.y = r.y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
        }
};

class Enemy : public GameObject
{
    public:
       
        ObjectType type = ObjectType::OEnemy;

        void draw(sf::RenderWindow* window)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(60, 60));
            rectangle.setFillColor(sf::Color::Red);
            rectangle.setPosition(r.x - 30, r.y - 30);

            window->draw(rectangle);
        }

        void move(float dT)
        {
            r.x = r.x + speedX * dT + accelerationX * dT * dT / 2;
            r.y = r.y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
        }
};

class Platform : public GameObject
{
    public:
        
        ObjectType type = ObjectType::OPlatform;

        void draw(sf::RenderWindow* window)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(80, 20));
            rectangle.setFillColor(sf::Color::Green);
            rectangle.setOutlineColor(sf::Color::White);
            rectangle.setOutlineThickness(5);
            rectangle.setPosition(r.x - 40, r.y);

            window->draw(rectangle);
        }

        void move(float dT)
        {
            r.x = r.x + speedX * dT + accelerationX * dT * dT / 2;
            r.y = r.y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
        }
};

class Projectile : public GameObject
{
    public:
        
        ObjectType type = ObjectType::OProjectile;

        int radius = 15;
        int colorRed = 0;
        int colorGreen = 255;
        int colorBlue = 0;
        int detailCirclesNum = 5;

        void draw(sf::RenderWindow* window)
        {
            for (int i = 0; i < detailCirclesNum; i++)
            {
                    sf::CircleShape circle(radius - i * radius / detailCirclesNum, 100);
                    circle.setPosition(r.x - radius + i * radius / detailCirclesNum, r.y - radius + i * radius / detailCirclesNum);
                    circle.setFillColor(sf::Color(colorRed * i / detailCirclesNum, colorGreen * i / detailCirclesNum, colorBlue * i / detailCirclesNum));
                    window->draw(circle);
            }
        }

        void move(float dT)
        {
            r.x = r.x + speedX * dT + accelerationX * dT * dT / 2;
            r.y = r.y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
        }
};

class LogicManager
{
    private:

        float score = 0;
        int highestPlatform = 25;
        int checkedPlat = 0;
        int scorePlat = -1;

    public:

        void moveAll(List<GameObject*> storage, float dT)
        {
            for (int i = 0; i < storage.length(); i++)
            {
                storage[i]->move(dT);

                if (storage[i]->type == ObjectType::OPlatform && storage[i]->r.y >= 800)
                {
                    storage[i]->r.x = storage[highestPlatform]->r.x - 150  + rand() % 300;

                    if (storage[i]->r.x >= 1000)
                    {
                        storage[i]->r.x -= 200;
                    }

                    else if (storage[i]->r.x <= 0)
                    {
                        storage[i]->r.x += 200;
                    }

                    storage[i]->r.y = storage[highestPlatform]->r.y - 200  + rand() % 100;

                    if (highestPlatform == 25)
                    {
                        highestPlatform = 0;
                    }

                    highestPlatform += 1;
                }
            }
        }

        void controlCharacter(List<GameObject*> storage)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                storage[0]->speedX = -2;
                storage[0]->orientaion = -2;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                storage[0]->speedX = 2;
                storage[0]->orientaion = 1;
            }

            else
            {
                storage[0]->speedX = 0;
            }
        }

        void collide(List<GameObject*> storage)
        {
            for (int i = 1; i < storage.length(); i++)
            {
                GameObject* itter = storage[i];
                if (itter->type == ObjectType::OPlatform)
                {
                    if (itter->r.y <= storage[0]->r.y && \
                    storage[0]->r.y <= itter->r.y + 5 && \
                    itter->r.x - 69 <= storage[0]->r.x && \
                    storage[0]->r.x <= itter->r.x + 69 && storage[0]->speedY > 0)
                    {
                        storage[0]->speedY = -10;

                        if (itter->r.y <= 450)
                        {
                            for (int j = 1; j < storage.length(); j++)
                            {
                                storage[j]->speedY = 10;
                            }

                            storage[0]->keptSpeedY = storage[0]->speedY;
                            storage[0]->speedY = 0;
                        }

                        checkedPlat = i;
                    }
                }
            }
        }

        void stopScreenMovement(List<GameObject*> storage)
        {
            if (checkedPlat > 0 && checkedPlat < storage.length())
            {
                if (storage[checkedPlat]->r.y >= 700)
                {
                    for (int j = 1; j < storage.length(); j++)
                    {
                        storage[j]->speedY = 0;
                    }
                }
            }
        }

        bool checkLoseConditions(List<GameObject*> storage)
        {
            if (storage[0]->r.y >= 800)
            {
                return true;
            }

            return false;
        }

        void updateScore(List<GameObject*> storage, float dT)
        {
            if ((scorePlat != checkedPlat && storage[0]->speedY <= 0) || storage[1]->speedY != 0)
            {
                score += 0.07;
            }

            else 
            {
                scorePlat = checkedPlat;
            }
        }

        sf::String getScore()
        {
            std::ostringstream textScore;
            textScore << int(score);

            return textScore.str();
        }
};

class GraphicsManager
{
    private:

        sf::Font font;

    public:

        void changeFont(std::string fontName)
        {
            font.loadFromFile(fontName);
        }


        void updateGraphics(LogicManager logics, List<GameObject*> storage, float dT, sf::RenderWindow* window)
        {
            window->clear();

            for (int i = 0; i < storage.length(); i++)
            {
                storage[i]->draw(window);
            }

            printScore(logics.getScore(), storage, dT, window);

            window->display();
        }

        void printScore(sf::String textScore, List<GameObject*> storage, float dT, sf::RenderWindow* window)
        {
            sf::Text score(textScore, font, 75);
            score.setPosition(100, 100);
            score.setFillColor(sf::Color::Red);
            window->draw(score);
        }
};
 
int main()
{
    List<GameObject*> storage;
    GraphicsManager graphics;
    LogicManager logics;

    graphics.changeFont("TimesNewRoman.ttf");

    Character doodle;
    doodle.r.x = 500;
    doodle.r.y = 760;
    doodle.accelerationY = 0.25;
   
    storage.append(&doodle);

    storage[0]->type = ObjectType::OCharacter;

    Platform platforms[25];

    platforms[0].r.x = 500;
    platforms[0].r.y = 760;

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

    float dT = 0.1;
    int checkedPlat;

    GameState state = GameState::NotStarted;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    graphics.updateGraphics(logics, storage, dT, &window);

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

        graphics.updateGraphics(logics, storage, dT, &window);

        if (logics.checkLoseConditions(storage))
        {
            return 0;
        }
    }

    return 0;
}