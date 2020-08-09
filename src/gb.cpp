#define _CRT_SECURE_NO_WARNINGS
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_memory_editor.h"
#include "PPU/Display.hpp"
#include "CPU/CPU.hpp"
#include "PPU/PPU.hpp"
#include "CPU/Joypad.hpp"
#include "util/ParseBinFile.hpp"
#include "util/Disassembler.hpp"
#include "ImguiDisassemblyViewer.hpp"
#include "ImguiRegistersWindow.hpp"
#include "ImguiPPUStateWindow.hpp"
#include <fstream>
#include <chrono>

void reset(CPU& cpu, MMU& mmu, PPU& ppu, std::vector<uint8_t>& rom) {
    std::cout << "\nResetting\n";
    cpu.reset();
    mmu.reset();
    ppu.reset();
    if (skipBootRom) {
        cpu.runUntilRomStart();
        needToRunBootRom = false;
    }
    else {
        needToRunBootRom = true;
    }
}

int main() {
    std::vector<uint8_t> rom = util::parseRomFile("./roms/tetris.gb");
    
    MMU mmu(rom);

    auto disassembly = util::disassemble(mmu.getMemory());
    
    Resolution r{ 1080, 560 };
    sf::RenderWindow w(sf::VideoMode(r.width, r.height), "gb");
    Display display(r, &w);

    Timers timers(&mmu.getMemory());
    PPU ppu(&mmu.getMemory(), &display);
    Joypad joypad(&mmu);
    CPU cpu(&mmu, &ppu, &timers, &joypad);

    static MemoryEditor memEdit;
    
    static RegistersWindow regWindow;

    static PPUStateWindow ppuStateWindow;

    static DisassemblyViewer disassemblyViewer(disassembly);

    // Run boot sequence
    if (skipBootRom) {
        cpu.runUntilRomStart();
    }

    //cpu.runUntilCompletion();
    int i = 0;
    while (display.isOpen()) {
        sf::Event event;
        while (display.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            
            if (event.type == sf::Event::Closed) {
                display.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::F7:
                    cpu.tick();
                    break;
                case sf::Keyboard::F1:
                    running = !running;
                    break;
                case sf::Keyboard::F2:
                    reset(cpu, mmu, ppu, rom);
                    running = false;
                    break;
                case sf::Keyboard::F3:
                    skipBootRom = !skipBootRom;
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
            }
            else if (event.type == sf::Event::KeyReleased) {
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
                }
            }
        }
        
        display.update();

        if (running) {
            cpu.runOneFrame();
        }

        // Some blargg tests have weird output locations, leave this for those
        //while (mmu.getMemory()[0xA004 + i] != '\0')
        //    std::cout << mmu.getMemory()[0xA004 + i++];

        memEdit.drawWindow("Memory Editor", mmu.getMemory().data(), 0x10000, 0x0000);
        disassemblyViewer.drawWindow(cpu);
        regWindow.drawWindow(cpu);
        ppuStateWindow.drawWindow(ppu);
        display.render(ppu.getFrame());
    }

    return EXIT_SUCCESS;
}