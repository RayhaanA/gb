#include "display.hpp"

Display::Display(Resolution r, sf::RenderWindow&& window) : windowSize(r), window(window)
{
    window.setSize(sf::Vector2u(r.width, r.height));    
}


Display::~Display()
{
}

void Display::render()
{
    window.clear();
    window.display();
}

bool Display::isOpen()
{
    return window.isOpen();
}

bool Display::pollEvent(sf::Event& event)
{
    return window.pollEvent(event);
}

void Display::close()
{
    window.close();
}