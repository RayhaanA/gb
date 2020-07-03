#define _CRT_SECURE_NO_WARNINGS
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_memory_editor.h"
#include "Display.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "util/ParseBinFile.hpp"
#include "util/Disassembler.hpp"
#include "ImguiDisassemblyViewer.hpp"
#include "ImguiRegistersWindow.hpp"
#include "ImguiPPUStateWindow.hpp"
#include <fstream>
#include <chrono>

void reset(CPU& cpu, MMU& mmu, PPU& ppu, std::vector<uint8_t>& rom) {
    std::cout << "Resetting\n";
    cpu.reset();
    mmu.reset();
    ppu.reset();
    cpu.runUntilRomStart();
}

int main() {
    // C505 , when AF is 3750 , issue at values at 0xDFFB|0xDFFC in stack : wrong value, check when it writes
    std::vector<uint8_t> rom = util::parseRomFile("./roms/09-op_r_r.gb");
    
    MMU mmu(rom);

    auto disassembly = util::disassemble(mmu.getMemory());
    
    Resolution r{ 1080, 560 };
    sf::RenderWindow w(sf::VideoMode(r.width, r.height), "gb");
    Display display(r, std::move(w));

    Timers timers(&mmu.getMemory());
    PPU ppu(&mmu.getMemory(), &display);
    CPU cpu(&mmu, &ppu, &timers);

    static MemoryEditor memEdit;
    
    static RegistersWindow regWindow;

    static PPUStateWindow ppuStateWindow;

    static DisassemblyViewer disassemblyViewer(disassembly);

    // Run boot sequence
    cpu.runUntilRomStart();

    cpu.runUntilCompletion();

    while (display.isOpen())
    {
        sf::Event event;
        while (display.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            
            if (event.type == sf::Event::Closed) {
                display.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::N) {
                    cpu.tick();
                }
                if (event.key.code == sf::Keyboard::R) {
                    reset(cpu, mmu, ppu, rom);
                }
            }
        }
        
        display.update();
        memEdit.drawWindow("Memory Editor", mmu.getMemory().data(), 0x10000, 0x0000);
        disassemblyViewer.drawWindow(cpu);
        //ImGui::ShowDemoWindow();
        regWindow.drawWindow(cpu);
        ppuStateWindow.drawWindow(ppu);
        display.render();
    }

    return EXIT_SUCCESS;
}