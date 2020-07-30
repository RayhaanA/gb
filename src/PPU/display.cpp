#include "Display.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
sf::Clock Display::clock = sf::Clock();

Display::Display(Resolution r, sf::RenderWindow* w) : windowSize(r) {
    window = std::unique_ptr<sf::RenderWindow>(w);
    window->setSize(sf::Vector2u(r.width, r.height));
    ImGui::SFML::Init(*window);
    window->setFramerateLimit(60);
}

Display::~Display() {
    window->close();
    window.release();
}

void Display::render(sf::Texture& frame) {
    clearWindow();
    ImGui::SFML::Render(*window);
    sf::Sprite s = sf::Sprite(frame);
    s.scale({ 2.0f, 2.0f });
    window->draw(s);
    window->display();
}

void Display::update() {
    ImGui::SFML::Update(*window, clock.restart());
}
