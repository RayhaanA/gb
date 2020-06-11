#define _CRT_SECURE_NO_WARNINGS

#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_memory_editor.h"
#include "Display.hpp"
#include "CPU.hpp"
#include "util/ParseBinFile.hpp"

int main()
{
    Resolution r{ 880, 432 };
    sf::RenderWindow w(sf::VideoMode(r.width, r.height), "gb");
    Display display(r, std::move(w));
    
    std::vector<uint8_t> rom = util::parseRomFile("./roms/dmg_boot.bin");
    MMU mmu(rom);
    CPU cpu(mmu);

    static MemoryEditor memEdit;

    mmu.printMemory(0, 255);


    util::diassemble(mmu.getMemory());
    //cpu.printRegisters();
    //while (cpu.getPC() != 0x64) {
        //std::cout << "ITERATION " << i << "====== START\n";
        //cpu.tick();
       // cpu.printRegisters();
       // std::cout << "ITERATION " << i << "====== END \n\n";
    //}
    //cpu.printRegisters();

    /*gb.callInstruction(0x3C);
    gb.callInstruction(0x01);
    gb.callInstruction(0x23);
    gb.callInstruction(0x09);
    std::cout << "\n";
    gb.printRegisters();*/


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
                    cpu.printRegisters();
                }
            }
        }

        display.update();
        memEdit.DrawWindow("Memory Editor", mmu.getMemory().data(), 0x1000, 0x0000);
        display.render();
    }

    return EXIT_SUCCESS;
}