#pragma once
#include  "Register.hpp"

const uint8_t zf = 1 << 7;
const uint8_t n = 1 << 6;
const uint8_t h = 1 << 5;
const uint8_t cy = 1 << 4;

class CPU
{
private:
    bool IME = false; // Interrupt Master Enable Flag: Set through op codes
    Register AF, BC, DE, HL, SP, PC;

public:
    // Register utilities
    uint8_t getA() { return AF.getHigh(); }
    uint8_t getFlags() { return AF.getLow(); }
    bool getZFlag() { return AF.getLow() & 0x80; }
    bool getNFlag() { return AF.getLow() & 0x40; }
    bool getHFlag() { return AF.getLow() & 0x20; }
    bool getCYFlag() { return AF.getLow() & 0x10; }
    void setZFlag() { AF.getLow() | 0x80; }
    void setNFlag() { AF.getLow() | 0x40; }
    void setHFlag() { AF.getLow() | 0x20; }
    void setCYFlag() { AF.getLow() | 0x10; }
    uint8_t getB() { return BC.getHigh(); }
    uint8_t getC() { return BC.getLow(); }
    uint8_t getD() { return DE.getHigh(); }
    uint8_t getE() { return DE.getLow(); }
    uint8_t getH() { return HL.getHigh(); }
    uint8_t getL() { return HL.getLow(); }
    uint16_t getSP() { return SP.getData(); }
    uint16_t getPC() { return PC.getData(); }
};
