#include "MMU.hpp"
#include "../Globals.hpp"
#include <iostream>

uint8_t MMU::read(uint16_t address, PPU& ppu) {
    return mbc->read(memory, address, ppu);
}

void MMU::write(uint8_t data, uint16_t address, PPU& ppu) {
    mbc->write(memory, data, address, ppu);
}
