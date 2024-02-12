#include "Display.hpp"

#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"

sf::Clock Display::clock = sf::Clock();

Display::Display(Resolution r, sf::RenderWindow* w) : windowSize(r) {
  window = std::unique_ptr<sf::RenderWindow>(w);
  window->setSize(sf::Vector2u(r.width, r.height));
  window->setVerticalSyncEnabled(false);
  ImGui::SFML::Init(*window);
  window->setFramerateLimit(fps);
}

Display::~Display() {
  window->close();
  window.release();
}

void Display::render(sf::Texture& frame) {
  clearWindow();
  ImGui::SFML::Render(*window);
  sf::Sprite s = sf::Sprite(frame);
  s.scale({Globals::DISPLAY_SCALE * 1.0f, Globals::DISPLAY_SCALE * 1.0f});
  window->draw(s);
  window->display();
}

void Display::update() { ImGui::SFML::Update(*window, clock.restart()); }
