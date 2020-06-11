#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include "./util/Disassembler.hpp"

class MMU
{
private:
    std::unordered_map<std::string, uint16_t> addressRanges
    {
        {"ROM0",  0x3FFF},
        {"ROMN", 0x7FFF},
        {"VRAM", 0x9FFF},
        {"ERAM",  0xBFFF},
        {"WRAM0", 0xCFFF},
        {"WRAM1",  0xDFFF},
        {"WRAM0_ECHO",  0xFDFF},
        {"SPRITE_TABLE",  0xFE9F},
        {"UNUSABLE",  0xFEFF},
        {"IO",  0xFF7F},
        {"HRAM",  0xFFFE},
        {"INTERRUPT_ENABLE",  0xFFFF}
    };

    /*
      General Memory Map
      0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
      4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
      8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
      A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
      C000-CFFF   4KB Work RAM Bank 0 (WRAM)
      D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
      E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
      FE00-FE9F   Sprite Attribute Table (OAM)
      FEA0-FEFF   Not Usable
      FF00-FF7F   I/O Ports
      FF80-FFFE   High RAM (HRAM)
      FFFF        Interrupt Enable Register
    */
    std::vector<uint8_t> memory;
    bool interruptEnableFlag = false;

public:
    MMU(std::vector<uint8_t> rom) {
        memory = std::vector<uint8_t>(0x10000);
        for (size_t i = 0; i < rom.size(); ++i) {
            memory[i] = rom[i];
        }
    }
    ~MMU() = default;

    void printMemory(size_t start, size_t end) {
        if (end > memory.size()) {
            std::cout << memory.size() << "\n";
            std::cerr << "Trying to print out of memory bounds!" << std::endl;
            return;
        }

        for (size_t i = start; i <= end; ++i) {
            if (i % 16 == 0) {
                std::cout << "\n";
            }

            if (i % 16 != 0) {
                std::cout << " ";
            }

            util::printHex(memory[i], 2);
        }
        std::cout << "\n";
    }

    std::vector<uint8_t>& getMemory() { return memory; }

    uint8_t read(uint16_t address, uint32_t& cycles);
    void write(uint8_t data, uint16_t address, uint32_t& cycles);
};

