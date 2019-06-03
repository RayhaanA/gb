#include "mmu.hpp"

MMU::MMU(std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m) : mmap(m)
{
}

MMU::~MMU()
{
}

uint8_t MMU::readMemory(uint16_t address)
{
    return mmap[address];
}

void MMU::writeMemory(uint8_t data, uint16_t address)
{
    mmap[address] = data;
}
