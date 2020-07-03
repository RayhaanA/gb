#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include "PPU.hpp"
#include "./util/Logging.hpp"
#include "./util/ParseBinFile.hpp"

class MMU
{
private:
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
    std::vector<uint8_t> bootAreaRemap;
    std::vector<uint8_t> memory;
    std::vector<uint8_t> rom;
public:
    MMU() = default;
    explicit MMU(std::vector<uint8_t> r) : rom(r) {
        bootAreaRemap = std::vector<uint8_t>(0x100);
        memory = std::vector<uint8_t>(0x10000);
        std::vector<uint8_t> bootRom = util::parseRomFile("./roms/dmg_boot.bin");
        for (size_t i = 0; i < bootRom.size(); ++i) {
            memory[i] = bootRom[i];
        }
        for (size_t i = 0; i < bootRom.size(); ++i) {
            bootAreaRemap[i] = rom[i];
        }
        for (size_t i = 0x100; i < rom.size(); ++i) {
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
    std::vector<uint8_t>& getBootAreaRemap() { return bootAreaRemap; }

    uint8_t read(uint16_t address, PPU& ppu);
    void write(uint8_t data, uint16_t address, PPU& ppu);
    uint8_t directRead(uint16_t address) { 
        if (address == IF_REG_ADDR) {
            return memory[address] | 0xE0;
        }
        return memory[address]; 
    }
    void directWrite(uint8_t data, uint16_t address) {
        if (address == IF_REG_ADDR) {
            memory[address] = data | 0xE0;
        }
        memory[address] = data;
    }


    void reset() {
        std::vector<uint8_t> bootRom = util::parseRomFile("./roms/dmg_boot.bin");
        for (size_t i = 0; i < bootRom.size(); ++i) {
            memory[i] = bootRom[i];
        }
        for (size_t i = 0x100; i < rom.size(); ++i) {
            memory[i] = rom[i];
        }
    }

    void loadRom(std::vector<uint8_t> rom) {
        for (size_t i = 0; i < rom.size(); ++i) {
            memory[i] = rom[i];
        }
    }
};

