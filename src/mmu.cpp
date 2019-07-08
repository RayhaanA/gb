#include "mmu.hpp"
#include <iostream>

MMU::MMU(std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m) : mmap(m)
{
}

MMU::~MMU()
{
}

uint8_t MMU::readMemory(uint16_t address)
{
    if (address < addressRanges["ROM0"]) { return rom0[address]; } 
    else if (address < addressRanges["ROMN"]) { return romN[address]; }
    else if (address < addressRanges["VRAM"]) { return vram[address]; }
    else if (address < addressRanges["ERAM"]) { return eram[address]; }
    else if (address < addressRanges["WRAM0"]) { return wram0[address]; }
    else if (address < addressRanges["WRAM1"]) { return wram1[address]; }
    else if (address < addressRanges["WRAM0_ECHO"]) { return wram0Echo[address]; }
    else if (address < addressRanges["SPRITE_TABLE"]) { return spriteTable[address]; }
    else if (address < addressRanges["UNSABLE"]) { std::cout << "Read from unsable address space!\n"; return 0; }
    else if (address < addressRanges["IO"]) { return io[address]; }
    else if (address < addressRanges["HRAM"]) { return hram[address]; }
    else if (address < addressRanges["INTERRUPT_ENABLE"]) { return interruptEnableFlag; }
    else
    {
        throw std::out_of_range("Tried to read from an out of range memory address!");
    }
}

void MMU::writeMemory(uint8_t data, uint16_t address)
{
    if (address < addressRanges["ROM0"]) { rom0[address] = data; }
    else if (address < addressRanges["ROMN"]) { romN[address] = data; }
    else if (address < addressRanges["VRAM"]) { vram[address] = data; }
    else if (address < addressRanges["ERAM"]) { eram[address] = data; }
    else if (address < addressRanges["WRAM0"]) { wram0[address] = data; }
    else if (address < addressRanges["WRAM1"]) { wram1[address] = data; }
    else if (address < addressRanges["WRAM0_ECHO"]) { wram0Echo[address] = data; }
    else if (address < addressRanges["SPRITE_TABLE"]) { spriteTable[address] = data; }
    else if (address < addressRanges["UNSABLE"]) { std::cout << "Wrote to unusable address space!\n"; return; }
    else if (address < addressRanges["IO"]) { io[address] = data; }
    else if (address < addressRanges["HRAM"]) { hram[address] = data; }
    else if (address < addressRanges["INTERRUPT_ENABLE"]) { interruptEnableFlag = data; }
    else
    {
        throw std::out_of_range("Tried to write to an out of range memory address!");
    }
}
