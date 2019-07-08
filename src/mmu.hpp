#pragma once

#include <array>
#include <unordered_map>

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
    std::array<uint8_t, 0x3FFF> rom0;
    std::array<uint8_t, 0x3FFF> romN;
    std::array<uint8_t, 0x1FFF> vram;
    std::array<uint8_t, 0x1FFF> eram;
    std::array<uint8_t, 0x0FFF> wram0;
    std::array<uint8_t, 0x0FFF> wram1;
    std::array<uint8_t, 0x1DFF> wram0Echo;
    std::array<uint8_t, 0x9F> spriteTable;
    std::array<uint8_t, 0x7F> io;
    std::array<uint8_t, 0x7E> hram;
    bool interruptEnableFlag = true;

public:
    MMU(std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m);
    ~MMU();

    uint8_t readMemory(uint16_t address);
    void writeMemory(uint8_t data, uint16_t address);
};

