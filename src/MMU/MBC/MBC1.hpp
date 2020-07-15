#pragma once
#include "MemoryController.hpp"
#include <vector>
#include <iostream>

class MBC1 : public MemoryController {
private:
    bool defaultMode = true;
public:
    MBC1(std::vector<uint8_t>& r, uint8_t romInfo, uint8_t ramInfo) : MemoryController(r, romInfo, ramInfo) {
        ramEnable = false;
    }
    ~MBC1() = default;

    uint8_t read(std::vector<uint8_t>& memory, uint16_t address, PPU& ppu) {
        switch (address & 0xF000) {
        case 0x0000: case 0x1000: case 0x2000: case 0x3000:
        case 0x8000: case 0x9000: case 0xC000: case 0xD000:
            return memory[address];
            break;
        case 0x4000: case 0x5000: case 0x6000: case 0x7000:
            return rom[address + romSize / numRomBanks * static_cast<size_t>((romBank % numRomBanks)- 1)];
        case 0xA000: case 0xB000:
            if (numRamBanks > 0 && ramEnable) {
                // choose bank 
                // if default mode then only ram bank 0 can be accessed

                return memory[address];
            }
            else {
                return UNDEFINED_READ;
            }
            break;
        case 0xE000:
            return memory[address - 0x2000];
            break;
        case 0xF000:
            switch (address & 0xF00) {
            case 0x000: case 0x100: case 0x200: case 0x300: case 0x400:
            case 0x500: case 0x600: case 0x700: case 0x800: case 0x900:
            case 0xA00: case 0xB00: case 0xC00: case 0xD00:
                return memory[address - 0x2000];
                break;
            case 0xE00:
                if ((address & 0xFF) < 0xA0) {
                    switch (ppu.getMode()) {
                    case PPUMode::OAM_SEARCH:
                    case PPUMode::DATA_TRANSFER:
                        return UNDEFINED_READ;
                        break;
                    default:
                        return memory[address];
                        break;
                    }
                }
                else {
                    return 0;
                }
                break;
            case 0xF00:
                if (address == IF_REG_ADDR) {
                    return memory[address] | 0xE0;
                }
                else if (address == 0xFF69 || address == 0xFF6B) {
                    if (ppu.getMode() == PPUMode::DATA_TRANSFER) {
                        return UNDEFINED_READ;
                    }
                }
                else {
                    switch (address & 0xF0) {
                    case 0x40:
                        return ppu.readRegisterValues(address);
                        break;
                    default:
                        return memory[address];
                    }
                }
                break;
            }
        default:
            std::cerr << "error with address " << std::hex << address << std::dec << std::endl;
            throw std::out_of_range("Tried to read from an out of range memory address");
            break;
        }
    }

    void write(std::vector<uint8_t>& memory, uint8_t data, uint16_t address, PPU& ppu) {
        if (address == 0xFF01)
            std::cout << data;

        switch (address & 0xF000) {
        case 0x0000: case 0x1000:
            if ((data & 0xF) == 0xA) {
                ramEnable = true;;
            }
            else {
                ramEnable = false;
            }
            break;
        case 0x2000: case 0x3000: 
            if (data == 0x0) {
                data = 0x1;
            }
            romBank = romBank & (~0x1F) | (data & 0x1F);
            break;
        case 0x4000: case 0x5000: 
            if (defaultMode) {
                romBank = romBank & 0x9F | ((data & 0x3) << 5);
            }
            else {
                ramBank = data & 0x3;
            }
            break;
        case 0x6000: case 0x7000:
            if (data == 0x0) {
                defaultMode = true;
                romBank |= (ramBank & 0x3) << 5;
            }
            else if (data == 0x1) {
                defaultMode = false;
                ramBank = (romBank & 0x60) >> 5;
                // In RAM banking mode, upper 2 bits of ROM bank are inaccessible
                romBank &= 0x1F;
            }
            break;
        case 0x8000: case 0x9000:
            memory[address] = data;
            break;
        case 0xA000: case 0xB000:
            if (numRamBanks > 0 && ramEnable) {
                memory[address] = data;
            }
            break;
        case 0xC000:
            memory[address] = data;
            memory[address + 0x2000] = data;
            break;
        case 0xD000:
            switch (address & 0x0F00) {
            case 0xD00:
                memory[address] = data;
                memory[address + 0x2000] = data;
                break;
            default:
                memory[address] = data;
                break;
            }
            break;
        case 0xE000:
            memory[address] = data;
            break;
        case 0xF000:
            switch (address & 0x0F00) {
            case 0x000: case 0x100: case 0x200: case 0x300: case 0x400:
            case 0x500: case 0x600: case 0x700: case 0x800: case 0x900:
            case 0xA00: case 0xB00: case 0xC00: case 0xD00:
                memory[address] = data;
                break;
            case 0xE00:
                if ((address & 0xFF) < 0xA0) {
                    switch (ppu.getMode()) {
                    case PPUMode::OAM_SEARCH:
                    case PPUMode::DATA_TRANSFER:
                        return;
                        break;
                    default:
                        memory[address] = data;
                        break;
                    }
                }
                break;
            case 0xF00:
                if (address == IE_REG_ADDR) {
                    IME = data;
                    memory[address] = data;
                }
                else if (address == DIV_REG_ADDR) {
                    // Reset div register if tried to write to
                    memory[address] = 0;
                }
                else if (address == IF_REG_ADDR) {
                    if ((data & 1) == 0) {
                        wroteZeroToVBLIF = true;
                    }

                    memory[address] = data | 0xE0;
                }
                else if (address == 0xFF69 || address == 0xFF6B) {
                    if (ppu.getMode() == PPUMode::DATA_TRANSFER) {
                        return;
                    }
                }
                else {
                    switch (address & 0xF0) {
                    case 0x40:
                        if (address == LY_REG_ADDR && !ppu.getDisplayEnabled()) {

                        }
                        else {
                            memory[address] = data;
                            ppu.updateRegistersValues(address);
                        }
                        break;
                    case 0x50: case 0x60: case 0x70:
                        memory[address] = data;
                        //ppu.updateRegisters(address);
                        break;
                    default:
                        memory[address] = data;
                        break;
                    }
                }
                break;
            }
            break;
        default:
            std::cerr << "error with address " << std::hex << address << std::dec << std::endl;
            throw std::out_of_range("Tried to write to an out of range memory address");
            break;
        }
    }
};
