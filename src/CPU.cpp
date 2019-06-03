#include "CPU.hpp"

void CPU::setFlags(uint8_t data)
{
    AF.setLow(data);
    flags.zf = data & 0x80;
    flags.n = data & 0x40;
    flags.h = data & 0x20;
    flags.cy = data & 0x10;
}