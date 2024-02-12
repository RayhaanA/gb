#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <fstream>

#include "CPU/CPU.hpp"
#include "CPU/Joypad.hpp"
#include "ImguiDisassemblyViewer.hpp"
#include "ImguiPPUStateWindow.hpp"
#include "ImguiRegistersWindow.hpp"
#include "PPU/Display.hpp"
#include "PPU/PPU.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_memory_editor.h"
#include "util/Disassembler.hpp"
#include "util/ParseBinFile.hpp"

void reset(CPU& cpu, MMU& mmu, PPU& ppu, std::vector<uint8_t>& rom) {
  std::cout << "\nResetting\n";
  cpu.reset();
  mmu.reset();
  ppu.reset();
  if (Globals::skipBootRom) {
    cpu.runUntilRomStart();
    Globals::needToRunBootRom = false;
  } else {
    Globals::needToRunBootRom = true;
  }
}

int main() {
  std::string file = "./roms/pokemon_red.gb";
  std::vector<uint8_t> rom = std::move(util::parseBinFile(file));
  std::cout << "Loaded file " << file << "\n";

  MMU mmu(&rom, file);

  Resolution resolution{
      static_cast<uint16_t>(Display::WIDTH * Globals::DISPLAY_SCALE),
      static_cast<uint16_t>(Display::HEIGHT * Globals::DISPLAY_SCALE)};
  sf::RenderWindow window(sf::VideoMode(resolution.width, resolution.height),
                          "gb", sf::Style::Titlebar | sf::Style::Close);
  Display display(resolution, &window);

  Timers timers(&mmu.getMemory());
  PPU ppu(&mmu.getMemory(), &display);
  Joypad joypad(&mmu);
  CPU cpu(&mmu, &ppu, &timers, &joypad);

  static MemoryEditor memEdit;

  static RegistersWindow regWindow;

  static PPUStateWindow ppuStateWindow;

  static std::unique_ptr<DisassemblyViewer> disassemblyViewer;

  // Run boot sequence
  if (Globals::skipBootRom) {
    std::cout << "Skipping boot sequence\n";
    cpu.runUntilRomStart();
  }

  int i = 0;
  while (display.isOpen()) {
    sf::Event event;
    while (display.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        display.close();
      } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::F7:
            cpu.tick();
            break;
          case sf::Keyboard::F1:
            std::cout << "Setting run state " << !Globals::running << "\n";
            Globals::running = !Globals::running;
            break;
          case sf::Keyboard::F2:
            reset(cpu, mmu, ppu, rom);
            Globals::running = false;
            break;
          case sf::Keyboard::F3:
            std::cout << "Should skip boot sequence " << !Globals::skipBootRom
                      << "\n";
            Globals::skipBootRom = !Globals::skipBootRom;
            break;
          case sf::Keyboard::Space:
            display.setFps(144);
            break;
          case sf::Keyboard::D:
            Globals::imgui_debug = !Globals::imgui_debug;
            if (Globals::imgui_debug) {
              auto disassembly = util::disassemble(mmu.getMemory());
              disassemblyViewer = std::make_unique<DisassemblyViewer>(std::move(disassembly));
              Globals::DISPLAY_SCALE = 4;
              resolution.width = Display::WIDTH * Globals::DISPLAY_SCALE +
                                 Display::DEBUG_WINDOW_SIZE;
              window.create(sf::VideoMode(resolution.width, resolution.height),
                            "gb", sf::Style::Titlebar | sf::Style::Close);
            } else {
              disassemblyViewer.release();
              resolution.width = Display::WIDTH * Globals::DISPLAY_SCALE;
              window.create(sf::VideoMode(resolution.width, resolution.height),
                            "gb", sf::Style::Titlebar | sf::Style::Close);
            }
            break;
          case sf::Keyboard::Equal:
            if (Globals::imgui_debug || Globals::DISPLAY_SCALE >= 8) break;
            ++Globals::DISPLAY_SCALE;
            window.setSize(
                sf::Vector2u(Display::WIDTH * Globals::DISPLAY_SCALE,
                             Display::HEIGHT * Globals::DISPLAY_SCALE));
            break;
          case sf::Keyboard::Dash:
            if (Globals::imgui_debug || Globals::DISPLAY_SCALE <= 1) break;
            --Globals::DISPLAY_SCALE;
            window.setSize(
                sf::Vector2u(Display::WIDTH * Globals::DISPLAY_SCALE,
                             Display::HEIGHT * Globals::DISPLAY_SCALE));
            break;
          case sf::Keyboard::Z:
          case sf::Keyboard::X:
          case sf::Keyboard::Enter:
          case sf::Keyboard::BackSpace:
          case sf::Keyboard::Right:
          case sf::Keyboard::Left:
          case sf::Keyboard::Up:
          case sf::Keyboard::Down:
            joypad.pressKey(event.key.code);
            break;
          default:
            break;
        }
      } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
          case sf::Keyboard::Z:
          case sf::Keyboard::X:
          case sf::Keyboard::Enter:
          case sf::Keyboard::BackSpace:
          case sf::Keyboard::Right:
          case sf::Keyboard::Left:
          case sf::Keyboard::Up:
          case sf::Keyboard::Down:
            joypad.releaseKey(event.key.code);
            break;
          case sf::Keyboard::Space:
            display.setFps(60);
            break;
          default:
            break;
        }
      }
    }

    display.update();

    if (Globals::running) {
      cpu.runOneFrame();
    }

    // Some blargg tests have weird output locations, leave this for those
    // while (mmu.getMemory()[0xA004 + i] != '\0')
    //  std::cout << mmu.getMemory()[0xA004 + i++];
    if (Globals::imgui_debug) {
      memEdit.drawWindow("Memory Editor", mmu.getMemory().data(), 0x10000,
                         0x0000);
      if (disassemblyViewer != nullptr) {
        disassemblyViewer->drawWindow(cpu);
      }
      regWindow.drawWindow(cpu);
      ppuStateWindow.drawWindow(ppu);
    }
    display.render(ppu.getFrame());
  }

  ImGui::SFML::Shutdown();
  return EXIT_SUCCESS;
}