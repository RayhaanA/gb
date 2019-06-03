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
    inline bool isOpen() { return window.isOpen(); }
    inline bool pollEvent(sf::Event& event) { return window.pollEvent(event); }
    inline void close() { window.close(); }
};

