#pragma once
#include "Display.hpp"
#include "CPU.hpp"
#include "util/ParseBinFile.hpp"

int main()
{
    Resolution r{ 160, 144 };
    sf::RenderWindow w(sf::VideoMode(r.width, r.height), "gb");
    Display window(r, std::move(w));
    
    std::vector<uint8_t> rom = util::parseRomFile("./roms/dmg_boot.bin");
    MMU mmu(rom);
    CPU cpu(mmu);

    cpu.printMemory(0, 255);
    cpu.printRegisters();
    while (cpu.getPC() != 0x64) {
        //std::cout << "ITERATION " << i << "====== START\n";
        cpu.tick();
       // cpu.printRegisters();
       // std::cout << "ITERATION " << i << "====== END \n\n";
    }
    cpu.printRegisters();

    /*gb.callInstruction(0x3C);
    gb.callInstruction(0x01);
    gb.callInstruction(0x23);
    gb.callInstruction(0x09);
    std::cout << "\n";
    gb.printRegisters();*/


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::N) {
                    cpu.tick();
                    cpu.printRegisters();
                }
            default:
                break;
            }
        }

        window.render();
    }

    return EXIT_SUCCESS;
}