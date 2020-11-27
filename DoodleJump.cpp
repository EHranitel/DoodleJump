#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
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
 
class GraphicsManager
{
    public:

        void updateGraphics(List<GameObject*> storage, sf::RenderWindow* window)
        {
            window->clear();

            for (int i = 0; i < storage.length(); i++)
            {
                storage.findByIndex(i)->draw(window);
            }

            window->display();
        }
};

class LogicManager
{
    public:

        int checkedPlat = 0;

        void moveAll(List<GameObject*> storage, float dT)
        {
            for (int i = 0; i < storage.length(); i++)
            {
                storage.findByIndex(i)->move(dT);
            }
        }

        void controlCharacter(List<GameObject*> storage)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                storage.findByIndex(0)->speedX = -2;
                storage.findByIndex(0)->orientaion = -2;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                storage.findByIndex(0)->speedX = 2;
                storage.findByIndex(0)->orientaion = 1;
            }

            else
            {
                storage.findByIndex(0)->speedX = 0;
            }
        }

        void collide(List<GameObject*> storage)
        {
            for (int i = 1; i < storage.length(); i++)
            {
                GameObject* itter = storage.findByIndex(i);
                if (itter->type == ObjectType::OPlatform)
                {
                    if (itter->r.y <= storage.findByIndex(0)->r.y && \
                    storage.findByIndex(0)->r.y <= itter->r.y + 5 && \
                    itter->r.x - 69 <= storage.findByIndex(0)->r.x && \
                    storage.findByIndex(0)->r.x <= itter->r.x + 69)
                    {
                        storage.findByIndex(0)->speedY = -10;

                        if (itter->r.y <= 450)
                        {
                            for (int j = 1; j < storage.length(); j++)
                            {
                                storage.findByIndex(j)->speedY = 10;
                            }

                            storage.findByIndex(0)->keptSpeedY = storage.findByIndex(0)->speedY;
                            storage.findByIndex(0)->speedY = 0;
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
                if (storage.findByIndex(checkedPlat)->r.y >= 700)
                {
                    for (int j = 1; j < storage.length(); j++)
                    {
                        storage.findByIndex(j)->speedY = 0;
                    }
                }
            }
        }

        bool checkLoseConditions(List<GameObject*> storage)
        {
            if (storage.findByIndex(0)->r.y >= 800)
            {
                return true;
            }

            return false;
        }
};
 
int main()
{
    List<GameObject*> storage;
    GraphicsManager graphics;
    LogicManager logics;

    Character doodle;
    doodle.r.x = 500;
    doodle.r.y = 760;
    doodle.accelerationY = 0.25;

    Enemy monster1;
    monster1.r.x = 200;
    monster1.r.y = 200;

    Platform plat1;
    plat1.r.x = 500;
    plat1.r.y = 760;
    
    Platform plat2;
    plat2.r.x = 500;
    plat2.r.y = 560;

    Platform plat3;
    plat3.r.x = 500;
    plat3.r.y = 360;

    Platform plat4;
    plat4.r.x = 500;
    plat4.r.y = 160;

    Platform plat5;
    plat5.r.x = 500;
    plat5.r.y = -40;
    
    Platform plat6;
    plat6.r.x = 500;
    plat6.r.y = -240;

    Platform plat7;
    plat7.r.x = 500;
    plat7.r.y = -440;

    Platform plat8;
    plat8.r.x = 500;
    plat8.r.y = -640;

    Projectile proj1;
    proj1.r.x = 0;
    proj1.r.y = 0;

    storage.append(&doodle);
    storage.findByIndex(0)->type = ObjectType::OCharacter;

    storage.append(&monster1);
    storage.findByIndex(1)->type = ObjectType::OEnemy;

    storage.append(&plat1);
    storage.findByIndex(2)->type = ObjectType::OPlatform;
    storage.append(&plat2);
    storage.findByIndex(3)->type = ObjectType::OPlatform;
    storage.append(&plat3);
    storage.findByIndex(4)->type = ObjectType::OPlatform;
    storage.append(&plat4);
    storage.findByIndex(5)->type = ObjectType::OPlatform;
    storage.append(&plat5);
    storage.findByIndex(6)->type = ObjectType::OPlatform;
    storage.append(&plat6);
    storage.findByIndex(7)->type = ObjectType::OPlatform;
    storage.append(&plat7);
    storage.findByIndex(8)->type = ObjectType::OPlatform;
    storage.append(&plat8);
    storage.findByIndex(9)->type = ObjectType::OPlatform;

    storage.append(&proj1);
    storage.findByIndex(10)->type = ObjectType::OProjectile;

    float dT = 0.1;
    int checkedPlat;

    GameState state = GameState::NotStarted;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    graphics.updateGraphics(storage, &window);

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

        graphics.updateGraphics(storage, &window);

        if (logics.checkLoseConditions(storage))
        {
            return 0;
        }
    }

    return 0;
}
