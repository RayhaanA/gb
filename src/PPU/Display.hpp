#pragma once
#include <SFML/Graphics.hpp>

#include "../Globals.hpp"

struct Resolution {
  uint16_t width;
  uint16_t height;
};

class Display {
 private:
  Resolution windowSize;
  std::unique_ptr<sf::RenderWindow> window;
  static sf::Clock clock;
  uint32_t fps = 60;

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
  void setFps(uint32_t newFps) {
    if (newFps != fps) {
      window->setFramerateLimit(newFps);
      fps = newFps;
    }
  }

  static const uint16_t WIDTH = 160;
  static const uint16_t HEIGHT = 144;
  static const uint16_t DEBUG_WINDOW_SIZE = 730;
  static const inline float DISASSEMBLY_WINDOW_W = 306.0f;
  static const inline float DISASSEMBLY_WINDOW_H = 244.0f;
  static const inline float MEMORY_VIEWER_WINDOW_W = 552.0f;
  static const inline float MEMORY_VIEWER_WINDOW_H = 293.0f;
  static const inline float PPU_STATE_WINDOW_W = 213.0f;
  static const inline float PPU_STATE_WINDOW_H = 285.0f;
  static const inline float REGISTERS_STATE_WINDOW_W = 342.0f;
  static const inline float REGISTERS_STATE_WINDOW_H = 283.0f;
  static const inline float DISASSEMBLY_WINDOW_X = 6000.f;
  static const inline float DISASSEMBLY_WINDOW_Y = 0.0f;
  static const inline float MEMORY_VIEWER_WINDOW_X = 815.0f;
  static const inline float MEMORY_VIEWER_WINDOW_Y = 0.0f;
  static const inline float PPU_STATE_WINDOW_X = 1156.0f;
  static const inline float PPU_STATE_WINDOW_Y = 293.0f;
  static const inline float REGISTERS_STATE_WINDOW_X = 815.0f;
  static const inline float REGISTERS_STATE_WINDOW_Y = 293.0f;
};
