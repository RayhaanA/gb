#pragma once
#include <SFML/Graphics.hpp>
#include "../Globals.hpp"

struct Resolution
{
    uint16_t width;
    uint16_t height;
};

class Display
{
private:
    Resolution windowSize;
    std::unique_ptr<sf::RenderWindow> window;
    static sf::Clock clock;

public:
    Display(Resolution r, sf::RenderWindow* w);
    ~Display();
    void render(sf::Texture& frame);
    void update();
    void clearWindow() { window->clear(sf::Color(0, 0, 0)); }
    inline bool isOpen() { return window->isOpen(); }
    inline bool pollEvent(sf::Event& event) { return window->pollEvent(event); }
    inline void close() { window->close(); }
    sf::Clock& getClock() { return clock; }
    sf::RenderWindow& getWindow() { return *window; }
};

