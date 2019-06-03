#pragma once
#include <inttypes.h>

const uint8_t zf = 1 << 7;
const uint8_t n = 1 << 6;
const uint8_t h = 1 << 5;
const uint8_t cy = 1 << 4;

struct Register
{
    uint16_t data;
    uint8_t high() { return data & 0xFF00; }
    uint8_t low() { return data & 0x00FF; }
    void setHigh(uint8_t value) { data |= value << 8; }
    void setLow(uint8_t value) { data |= value; }
    void setData(uint16_t value) { data = value; }
};

struct Flags
{
    bool zf;
    bool n;
    bool h;
    bool cy;
};

class CPU
{
private:
    bool IME = false; // Interrupt Master Enable Flag: Set through op codes
    Register AF, BC, DE, HL, SP, PC;
    Flags flags;

public:
    // Register utilities
    uint8_t getA() { return AF.high(); }
    uint8_t getFlags() { return AF.low(); }
    void setFlags(uint8_t data);
    uint8_t getB() { return BC.high(); }
    uint8_t getC() { return BC.low(); }
    uint8_t getD() { return DE.high(); }
    uint8_t getE() { return DE.low(); }
    uint8_t getH() { return HL.high(); }
    uint8_t getL() { return HL.low(); }
    uint16_t getSP() { return SP.data; }
    uint16_t getPC() { return PC.data; }
};
