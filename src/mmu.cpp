#include "MMU.hpp"
#include <iostream>

uint8_t MMU::read(uint16_t address, uint32_t& cycles)
{
    ++cycles;
    if (address <= addressRanges["ROM0"]) { return memory[address]; } 
    else if (address <= addressRanges["ROMN"]) { return memory[address]; }
    else if (address <= addressRanges["VRAM"]) { return memory[address]; }
    else if (address <= addressRanges["ERAM"]) { return memory[address]; }
    else if (address <= addressRanges["WRAM0"]) { return memory[address]; }
    else if (address <= addressRanges["WRAM1"]) { return memory[address]; }
    else if (address <= addressRanges["WRAM0_ECHO"]) { return memory[address]; }
    else if (address <= addressRanges["SPRITE_TABLE"]) { return memory[address]; }
    else if (address <= addressRanges["UNSABLE"]) { std::cout << "Read from unsable address space at address " << std::hex << address << std::dec << "\n"; return 0; }
    else if (address <= addressRanges["IO"]) { std::cout << "Reading IO range\n";  return memory[address]; }
    else if (address <= addressRanges["HRAM"]) { return memory[address]; }
    else if (address <= addressRanges["INTERRUPT_ENABLE"]) { return interruptEnableFlag; }
    else
    {
        throw std::out_of_range("Tried to read from an out of range memory address\n");
    }
}

void MMU::write(uint8_t data, uint16_t address, uint32_t& cycles)
{
    ++cycles;
    if (address <= addressRanges["ROM0"]) { std::cout << "Tried to write to to ROM at " << std::hex << address << std::dec << "\n"; return; }
    else if (address <= addressRanges["ROMN"]) { std::cout << "Tried to write to to ROM at " << std::hex << address << std::dec << "\n";; return; }
    else if (address <= addressRanges["VRAM"]) { memory[address] = data; }
    else if (address <= addressRanges["ERAM"]) { memory[address] = data; }
    else if (address <= addressRanges["WRAM0"]) { memory[address] = data; }
    else if (address <= addressRanges["WRAM1"]) { memory[address] = data; }
    else if (address <= addressRanges["WRAM0_ECHO"]) { memory[address] = data; }
    else if (address <= addressRanges["SPRITE_TABLE"]) { memory[address] = data; }
    else if (address <= addressRanges["UNSABLE"]) { std::cout << "Tried to write to unusable address space at address " << std::hex << address << std::dec << "\n"; return; }
    else if (address <= addressRanges["IO"]) { std::cout << "Writing to address in IO range\n"; memory[address] = data; }
    else if (address <= addressRanges["HRAM"]) { memory[address] = data; }
    else if (address <= addressRanges["INTERRUPT_ENABLE"]) { interruptEnableFlag = data; }
    else
    {
        std::cout << "Tried to write to an out of range memory address " << std::hex << address << std::dec << "\n";
    }
}
