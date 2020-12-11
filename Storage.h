#ifndef Storage_H
#define Storage_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vector2f.h"
#include "List.h"

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

        int orientation = 0;

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

        void draw(sf::RenderWindow* window)
        {
            sf::RectangleShape body;
            body.setSize(sf::Vector2f(60, 60));
            body.setFillColor(sf::Color::Green);
            body.setPosition(r.x - 30, r.y - 30);

            sf::RectangleShape weapon;
            weapon.setFillColor(sf::Color::Green);
            
            if (orientation == 0)
            {
                weapon.setSize(sf::Vector2f(30, 10));
                weapon.setPosition(r.x + 30, r.y - 5);
            }
            
            else if (orientation == 1)
            {
                weapon.setSize(sf::Vector2f(30, 10));
                weapon.setPosition(r.x - 60, r.y - 5);
            }

            else if (orientation == 2)
            {
                weapon.setSize(sf::Vector2f(10, 30));
                weapon.setPosition(r.x - 5, r.y - 60);
            }            

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

            if (r.x <= 0 || r.x >= 1000)
            {
                speedX = -speedX;
            }

            if (r.y >= 900)
            {
                r.y = - 500  + rand() % 300;
            }

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
            rectangle.setPosition(r.x - 40, r.y - 10);

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
    private:

        int radius = 15;
        int colorRed = 0;
        int colorGreen = 255;
        int colorBlue = 0;
        int detailCirclesNum = 5;

    public:
        
        ObjectType type = ObjectType::OProjectile;

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

#endif