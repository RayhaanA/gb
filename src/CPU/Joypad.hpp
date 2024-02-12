#pragma once
#include <memory>

#include "../MMU/mmu.hpp"

class Joypad {
  std::shared_ptr<MMU> memory;
  const uint8_t DEFAULT_JOYPAD_VALUE = 0xFF;
  enum class JoypadButtons {
    JOYPAD_RIGHT,
    JOYPAD_LEFT,
    JOYPAD_UP,
    JOYPAD_DOWN,
    JOYPAD_A,
    JOYPAD_B,
    JOYPAD_SELECT,
    JOYPAD_START
  };

  const uint8_t SELECT_BUTTONS = 0x20;
  const uint8_t SELECT_DIRECTION_KEYS = 0x10;
  uint8_t state = 0xFF;
  const uint8_t JOYPAD_INTERRUPT_FLAG = 1 << 4;

 public:
  Joypad() = delete;
  explicit Joypad(MMU* mmu) { memory = std::shared_ptr<MMU>(mmu); }
  ~Joypad() { memory.reset(); }

  uint8_t readJoypadState() {
    uint8_t currValue = memory->getMemory()[Address::JOYPAD_REG_ADDR];
    currValue ^= 0xFF;

    if (!(currValue & SELECT_DIRECTION_KEYS)) {
      uint8_t msb = state >> 4;
      msb |= 0xF0;
      currValue &= msb;
    } else if (!(currValue & SELECT_BUTTONS)) {
      uint8_t lsb = state & 0xF;
      lsb |= 0xF0;
      currValue &= lsb;
    }

    return currValue;
  }

  void pressKey(sf::Keyboard::Key key) {
    bool wasUnset = false;
    uint8_t keyMap = 0;

    switch (key) {
      case sf::Keyboard::Z:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_A);
        break;
      case sf::Keyboard::X:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_B);
        break;
      case sf::Keyboard::Enter:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_START);
        break;
      case sf::Keyboard::BackSpace:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_SELECT);
        break;
      case sf::Keyboard::Right:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_RIGHT);
        break;
      case sf::Keyboard::Left:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_LEFT);
        break;
      case sf::Keyboard::Up:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_UP);
        break;
      case sf::Keyboard::Down:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_DOWN);
        break;
    }

    if (state & (1 << keyMap)) {
      wasUnset = true;
    }

    state &= (~(1 << keyMap));

    bool buttonPressed = keyMap > 3;

    uint8_t joypad = memory->getMemory()[Address::JOYPAD_REG_ADDR];

    bool requestInterrupt = false;
    if ((buttonPressed && !(joypad & SELECT_BUTTONS)) ||
        (!buttonPressed && !(joypad & SELECT_DIRECTION_KEYS))) {
      requestInterrupt = true;
    }

    if (requestInterrupt && !wasUnset) {
      memory->getMemory()[Address::IF_REG_ADDR] |= JOYPAD_INTERRUPT_FLAG & 0x1F;
    }
  }

  void releaseKey(sf::Keyboard::Key key) {
    uint8_t keyMap = 0;

    switch (key) {
      case sf::Keyboard::Z:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_A);
        break;
      case sf::Keyboard::X:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_B);
        break;
      case sf::Keyboard::Enter:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_START);
        break;
      case sf::Keyboard::BackSpace:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_SELECT);
        break;
      case sf::Keyboard::Right:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_RIGHT);
        break;
      case sf::Keyboard::Left:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_LEFT);
        break;
      case sf::Keyboard::Up:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_UP);
        break;
      case sf::Keyboard::Down:
        keyMap = static_cast<uint8_t>(JoypadButtons::JOYPAD_DOWN);
        break;
    }
    state |= (1 << keyMap);
  }
};