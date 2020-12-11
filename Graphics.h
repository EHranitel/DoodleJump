#ifndef Graphics_H
#define Graphics_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Storage.h"

class GraphicsManager
{
    private:

        sf::Font font;

    public:

        void changeFont(std::string fontName)
        {
            font.loadFromFile(fontName);
        }


        void updateGraphics(sf::String textScore, List<GameObject*> storage, float dT, sf::RenderWindow* window)
        {
            window->clear();

            for (int i = 0; i < storage.length(); i++)
            {
                storage[i]->draw(window);
            }

            printScore(textScore, storage, dT, window);

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

#endif