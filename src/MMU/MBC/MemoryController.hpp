#pragma once
#include "../../PPU/PPU.hpp"

class MemoryController {
protected:
    std::unique_ptr<std::vector<uint8_t>> rom;
    size_t romSize = 0;
    uint16_t numRomBanks = 0;
    size_t ramSize = 0;
    uint16_t numRamBanks = 0;
    uint16_t romBank = 1;
    uint16_t ramBank = 0;
public:
    explicit MemoryController(std::vector<uint8_t>* r, uint8_t romInfo, uint8_t ramInfo) {
        rom = std::unique_ptr<std::vector<uint8_t>>(r);
        romSize = BASE_ROM_SIZE << romInfo;
        numRomBanks = 2 << romInfo;

        switch (ramInfo) {
        case 0x1:
            ramSize = 2_KB;
            break;
        case 0x2:
            ramSize = 8_KB;
            numRamBanks = 1;
            break;
        case 0x3:
            ramSize = 32_KB;
            numRamBanks = 4;
            break;
        case 0x4:
            ramSize = 128_KB;
            numRamBanks = 16;
            break;
        case 0x5:
            ramSize = 64_KB;
            numRamBanks = 8;
            break;
        }
    }
    ~MemoryController() {
        rom.release();
    }
    virtual uint8_t read(std::vector<uint8_t>& memory, uint16_t address, PPU& ppu) = 0;
    virtual void write(std::vector<uint8_t>& memory, uint8_t data, uint16_t address, PPU& ppu) = 0;
};