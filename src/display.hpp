#pragma once
#include <SFML/Graphics.hpp>

struct Resolution
{
    uint16_t width;
    uint16_t height;
};

class Display
{
private:
    Resolution windowSize;
    sf::RenderWindow& window;

public:
    Display(Resolution r, sf::RenderWindow&& window);
    ~Display();
    void render();
    bool isOpen();
    bool pollEvent(sf::Event& event);
    void close();
};

