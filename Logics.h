#ifndef Logics_H
#define Logics_H

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
#include "Storage.h"

class LogicManager
{
    private:

        float score = 0;
        int highestPlatform = 25;
        int checkedPlat = 0;
        int scorePlat = -1;

    public:

        void deleteBottomPlatform(List<GameObject*> storage, int i)
        {
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

        void moveAll(List<GameObject*> storage, float dT)
        {
            for (int i = 0; i < storage.length(); i++)
            {
                storage[i]->move(dT);

                deleteBottomPlatform(storage, i);

                if (storage[i]->type == ObjectType::OProjectile && (storage[i]->r.y <= 0 || storage[i]->r.x <= 0 || storage[i]->r.x >= 1000))
                {
                    storage.removeByIndex(i);
                }
            }
        }

        void controlCharacter(List<GameObject*> storage)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                storage[0]->speedX = -2;
                storage[0]->orientation = 1;
            }
            
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                storage[0]->speedX = 2;
                storage[0]->orientation = 0;
            }

            else
            {
                storage[0]->speedX = 0;
            }

        }

        void shoot(List<GameObject*> storage)
        {
            storage[0]->orientation = 2;

            storage.append(createProjectile(storage));

            storage[storage.length() - 1]->type = ObjectType::OProjectile;
        }

        Projectile* createProjectile(List<GameObject*> storage)
        {
            Projectile* proj = new Projectile;
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                proj->r.x = storage[0]->r.x - 90;
                proj->r.y = storage[0]->r.y - 35;

                proj->speedX = -10;
                proj->speedY = storage[0]->speedY;

                return proj;
            }
            
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                proj->r.x = storage[0]->r.x + 60;
                proj->r.y = storage[0]->r.y - 35;
                
                proj->speedX = 10;
                proj->speedY = storage[0]->speedY;

                return proj;
            }         

            proj->r.x = storage[0]->r.x;
            proj->r.y = storage[0]->r.y - 90;

            proj->speedX = storage[0]->speedX;
            proj->speedY = -10;

            storage[0]->orientation = 2;

            return proj;
        } 

        void shootEnemy(List<GameObject*> storage, GameObject* monster, int i)
        {
            for (int j = 1; j < storage.length(); j++)
            {
                if (storage[j]->type == ObjectType::OProjectile && \
                abs(monster->r.x - storage[j]->r.x) <= 45 && \
                abs(monster->r.y - storage[j]->r.y) <= 45)
                {
                    monster->r.y = - 500  + rand() % 300;

                    storage.removeByIndex(j);

                    return;
                }
            }
        }

        void collide(List<GameObject*> storage)
        {
            for (int i = 1; i < storage.length(); i++)
            {
                GameObject* itter = storage[i];
                if (itter->type == ObjectType::OPlatform)
                {
                    collidewithPlatform(storage, itter, i);
                }

                else if (itter->type == ObjectType::OEnemy)
                {
                    shootEnemy(storage, itter, i);
                }
            }
        }

        void collidewithPlatform(List<GameObject*> storage, GameObject* platform, int i)
        {
            if (abs(platform->r.x - storage[0]->r.x) <= 70 && \
            abs(platform->r.y - storage[0]->r.y) <= 40 && \
            storage[0]->speedY >= 0)
            {
                storage[0]->speedY = -10;

                moveScreen(storage, platform);

                checkedPlat = i;
            }
        }

        void moveScreen(List<GameObject*> storage, GameObject* touchedPlatform)
        {
            if (touchedPlatform->r.y <= 450)
            {
                for (int j = 1; j < storage.length(); j++)
                {
                    if (storage[j]->type != ObjectType::OProjectile)
                    {
                        storage[j]->speedY = 10;
                    }
                }

                storage[0]->keptSpeedY = storage[0]->speedY;
                storage[0]->speedY = 0;
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
                        if (storage[j]->type != ObjectType::OProjectile)
                        {
                            storage[j]->speedY = 0;
                        }
                    }
                }
            }
        }
        
        bool loseConditions(List<GameObject*> storage)
        {
            if (storage[0]->r.y >= 800)
            {
                return true;
            }

            for (int i = 1; i < storage.length(); i++)
            {
                if (storage[i]->type == ObjectType::OEnemy && \
                abs(storage[0]->r.x - storage[i]->r.x) <= 60 && \
                abs(storage[0]->r.y - storage[i]->r.y) <= 60)
                {
                    return true;
                }
            }

            return false;
        }

        bool checkLoseConditions(List<GameObject*> storage)
        {
            if (loseConditions(storage))
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


#endif