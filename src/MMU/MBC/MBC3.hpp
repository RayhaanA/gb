#pragma once
#include <iostream>
#include <vector>

#include "MemoryController.hpp"

class MBC3 : public MemoryController {
 private:
  bool defaultMode = true;

 public:
  MBC3(std::vector<uint8_t>* r, uint8_t romInfo, uint8_t ramInfo)
      : MemoryController(r, romInfo, ramInfo) {
    Globals::ramEnable = false;
  }
  ~MBC3() = default;

  uint8_t read(std::vector<uint8_t>& memory, uint16_t address, PPU& ppu) {
    switch (address & 0xF000) {
      case 0x0000:
      case 0x1000:
      case 0x2000:
      case 0x3000:
      case 0xC000:
      case 0xD000:
        return memory[address];
        break;
      case 0x8000:
      case 0x9000:
        switch (ppu.getMode()) {
          case PPUMode::DATA_TRANSFER:
            return Globals::UNDEFINED_READ;
            break;
          default:
            return memory[address];
            break;
        }
        break;
      case 0x4000:
      case 0x5000:
      case 0x6000:
      case 0x7000:
        return (*rom)[(address) +
                      romSize / numRomBanks *
                          static_cast<size_t>((romBank % numRomBanks) - 1)];
      case 0xA000:
      case 0xB000:
        if (numRamBanks > 0 && Globals::ramEnable) {
          // choose bank
          // if default mode then only ram bank 0 can be accessed

          return memory[address];
        } else {
          return Globals::UNDEFINED_READ;
        }
        break;
      case 0xE000:
        return memory[address - 0x2000];
        break;
      case 0xF000:
        switch (address & 0xF00) {
          case 0x000:
          case 0x100:
          case 0x200:
          case 0x300:
          case 0x400:
          case 0x500:
          case 0x600:
          case 0x700:
          case 0x800:
          case 0x900:
          case 0xA00:
          case 0xB00:
          case 0xC00:
          case 0xD00:
            return memory[address - 0x2000];
            break;
          case 0xE00:
            if ((address & 0xFF) < 0xA0) {
              switch (ppu.getMode()) {
                case PPUMode::OAM_SEARCH:
                case PPUMode::DATA_TRANSFER:
                  return Globals::UNDEFINED_READ;
                  break;
                default:
                  return memory[address];
                  break;
              }
            } else {
              return Globals::UNDEFINED_READ;
            }
            break;
          case 0xF00:
            if (address == Address::IF_REG_ADDR) {
              return memory[address] | 0xE0;
            }
            // GBC Only?
            /*else if (address == 0xFF69 || address == 0xFF6B) {
                if (ppu.getMode() == PPUMode::DATA_TRANSFER) {
                    return UNDEFINED_READ;
                }
            }*/
            else {
              switch (address & 0xF0) {
                case 0x40:
                  if (address == Address::DMA_TRANSFER_ADDR ||
                      address > 0xFF4B) {
                    return memory[address];
                  } else {
                    return ppu.readRegisterValues(address);
                  }
                  break;
                default:
                  return memory[address];
              }
            }
            break;
        }
      default:
        std::cerr << "error with address " << std::hex << address << std::dec
                  << std::endl;
        throw std::out_of_range(
            "Tried to read from an out of range memory address");
        break;
    }
  }

  void write(std::vector<uint8_t>& memory, uint8_t data, uint16_t address,
             PPU& ppu) {
    switch (address & 0xF000) {
      case 0x0000:
      case 0x1000:
        if ((data & 0xF) == 0xA) {
          Globals::ramEnable = true;
        } else {
          Globals::ramEnable = false;
        }
        break;
      case 0x2000:
      case 0x3000: {
        if (data == 0x0) {
          data = 0x1;
        }
        uint8_t romBankNumber = data & 0x3F;
        romBank = romBankNumber;
        break;
      }
      case 0x4000:
      case 0x5000:
        if (defaultMode && romSize >= 1024_KB) {
          romBank = ((data & 0x3) << 5) + romBank;
        } else if (!defaultMode && ramSize >= 32_KB) {
          uint8_t ramBank = data & 0x0F;
          if (data <= 0x03)
            ramBank = data;
          else if (data <= 0x0C)
            throw std::runtime_error("RTC Unsupported");
        }
        break;
      case 0x6000:
      case 0x7000:
        if (ramSize <= 8_KB && romSize < 512_KB) {
          return;
        }
        if (data & 0x1) {
          defaultMode = false;
          ramBank = (romBank & 0x60) >> 5;
          // In RAM banking mode, upper 2 bits of ROM bank are inaccessible
          romBank &= 0x1F;
        } else {
          defaultMode = true;
          romBank |= (ramBank & 0x3) << 5;
        }
        break;
      case 0x8000:
      case 0x9000:
        if (ppu.getMode() == PPUMode::DATA_TRANSFER) {
          break;
        }
        memory[address] = data;
        break;
      case 0xA000:
      case 0xB000:
        if (numRamBanks > 0 && Globals::ramEnable) {
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
          case 0x000:
          case 0x100:
          case 0x200:
          case 0x300:
          case 0x400:
          case 0x500:
          case 0x600:
          case 0x700:
          case 0x800:
          case 0x900:
          case 0xA00:
          case 0xB00:
          case 0xC00:
          case 0xD00:
            memory[address] = data;
            break;
          case 0xE00:
            if ((address & 0xFF) < 0xA0) {
              switch (ppu.getMode()) {
                case PPUMode::OAM_SEARCH:
                case PPUMode::DATA_TRANSFER:
                  break;
                default:
                  memory[address] = data;
                  break;
              }
            }
            break;
          case 0xF00:
            if (address == Address::IE_REG_ADDR) {
              Globals::IME = data;
              memory[address] = data;
            } else if (address == Address::DIV_REG_ADDR) {
              // Reset div register if tried to write to
              Globals::resetSysCounter = true;
              memory[address - 1] = 0;
              memory[address] = 0;
            } else if (address == Address::IF_REG_ADDR) {
              if ((data & 1) == 0) {
                Globals::wroteZeroToVBLIF = true;
              }

              memory[address] = data | 0xE0;
            } else if (address == 0xFF69 || address == 0xFF6B) {
              if (ppu.getMode() == PPUMode::DATA_TRANSFER) {
                return;
              }
            } else {
              switch (address & 0xF0) {
                case 0x40:
                  if (address == Address::LY_REG_ADDR &&
                      !ppu.getDisplayEnabled()) {
                    break;
                  } else if (address == Address::DMA_TRANSFER_ADDR) {
                    memory[address] = data;
                  } else if (address > 0xFF4B) {
                    memory[address] = data;
                  } else {
                    memory[address] = data;
                    ppu.writeRegistersValues(address);
                  }
                  break;
                case 0x50:
                case 0x60:
                case 0x70:
                  memory[address] = data;
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
        std::cerr << "error with address " << std::hex << address << std::dec
                  << std::endl;
        throw std::out_of_range(
            "Tried to write to an out of range memory address");
        break;
    }
  }
};
