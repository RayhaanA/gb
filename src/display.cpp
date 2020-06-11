#include "Display.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

sf::Clock Display::clock = sf::Clock();

Display::Display(Resolution r, sf::RenderWindow&& window) : windowSize(r), window(window) {
    window.setSize(sf::Vector2u(r.width, r.height)); 
    ImGui::SFML::Init(window);
    window.resetGLStates();
}

Display::~Display() {
}

void Display::render() {
    window.clear();
    ImGui::SFML::Render(window);
    window.display();
}

void Display::update() {
    ImGui::SFML::Update(window, clock.restart());
}
