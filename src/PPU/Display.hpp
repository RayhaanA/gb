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
    static sf::Clock clock;

public:
    Display(Resolution r, sf::RenderWindow&& window);
    ~Display();
    void render();
    void update();
    void clearWindow() { window.clear(sf::Color(0, 0, 0)); }
    inline bool isOpen() { return window.isOpen(); }
    inline bool pollEvent(sf::Event& event) { return window.pollEvent(event); }
    inline void close() { window.close(); }
    sf::Clock& getClock() { return clock; }
    sf::RenderWindow& getWindow() { return window; }
};

