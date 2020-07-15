#pragma once
#include "../../PPU/PPU.hpp"

class MemoryController {
protected:
    std::vector<uint8_t> rom;
public:
    explicit MemoryController(std::vector<uint8_t>& r) : rom(r) {}
    virtual uint8_t read(std::vector<uint8_t>& memory, uint16_t address, PPU& ppu) = 0;
    virtual void write(std::vector<uint8_t>& memory, uint8_t data, uint16_t address, PPU& ppu) = 0;
};