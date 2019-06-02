#pragma once
#include <iostream>
#include "display.hpp"

int main()
{
    Resolution r{ 160, 144 };
    sf::RenderWindow w(sf::VideoMode(r.width, r.height), "gb");
    Display window(r, std::move(w));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.render();
    }

    return EXIT_SUCCESS;
}