#pragma once
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../PPU/PPU.hpp"
#include "../util/Logging.hpp"
#include "../util/ParseBinFile.hpp"
#include "MBC/MBC1.hpp"
#include "MBC/MBC3.hpp"
#include "MBC/NoMBC.hpp"

class MMU {
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
  std::vector<uint8_t> bootAreaRemap;
  std::vector<uint8_t> memory;
  std::shared_ptr<std::vector<uint8_t>> rom;
  std::string file;

  std::unique_ptr<MemoryController> mbc;
  uint8_t romSize = 0;
  const size_t BOOT_ROM_SIZE = 0x100;
  const size_t MEMORY_SIZE = 64_KB;

  const uint16_t CART_HEADER_MBC = 0x147;
  const uint16_t CART_HEADER_ROM_SIZE = 0x148;
  const uint16_t CART_HEADER_RAM_SIZE = 0x149;

  const uint8_t BOOT_ROM[256] = {
      0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB,
      0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
      0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 0x47, 0x11, 0x04, 0x01,
      0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
      0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22,
      0x23, 0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
      0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18,
      0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
      0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20,
      0xF7, 0x1D, 0x20, 0xF2, 0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
      0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 0x7B, 0xE2, 0x0C, 0x3E,
      0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
      0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17,
      0xC1, 0xCB, 0x11, 0x17, 0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
      0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
      0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
      0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
      0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
      0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 0x21, 0x04, 0x01, 0x11,
      0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x00, 0x00, 0x23, 0x7D, 0xFE, 0x34, 0x20,
      0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x00, 0x00,
      0x3E, 0x01, 0xE0, 0x50};

 public:
  MMU() = default;
  explicit MMU(std::vector<uint8_t>* r, std::string f) : rom(std::shared_ptr<std::vector<uint8_t>>(r)), file(f) {
    bootAreaRemap = std::vector<uint8_t>(BOOT_ROM_SIZE);
    memory = std::vector<uint8_t>(MEMORY_SIZE);
    for (size_t i = 0; i < BOOT_ROM_SIZE; ++i) {
      memory[i] = BOOT_ROM[i];
    }
    for (size_t i = 0; i < BOOT_ROM_SIZE; ++i) {
      bootAreaRemap[i] = rom->at(i);
    }
    for (size_t i = BOOT_ROM_SIZE; i < MEMORY_SIZE; ++i) {
      memory[i] = rom->at(i);
    }

    switch (memory[CART_HEADER_MBC]) {
      case 0x0:
        std::cout << "No MBC\n";
        mbc = std::unique_ptr<MemoryController>(
            new NoMBC(&(*rom), memory[CART_HEADER_ROM_SIZE],
                      memory[CART_HEADER_RAM_SIZE], file));
        break;
      case 0x1:
        std::cout << "MBC1\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC1(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], false, file));
        break;
      case 0x2:
        std::cout << "MBC1 with RAM\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC1(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], false, file));
        break;
      case 0x3:
        std::cout << "MBC1 with RAM and battery\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC1(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], true, file));
        break;
      case 0x11:
        std::cout << "MBC3\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC3(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], false, file));
        break;
      case 0x12:
        std::cout << "MBC3 with RAM\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC3(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], false, file));
        break;
      case 0x13:
        std::cout << "MBC3 with RAM and battery\n";
        mbc = std::unique_ptr<MemoryController>(
            new MBC3(&(*rom), memory[CART_HEADER_ROM_SIZE],
                     memory[CART_HEADER_RAM_SIZE], true, file));
        mbc->readRamFromFile(memory);
        break;
      default:
        mbc = std::unique_ptr<MemoryController>(
            new NoMBC(&(*rom), memory[CART_HEADER_ROM_SIZE],
                      memory[CART_HEADER_RAM_SIZE], file));
        break;
    }
  }
  ~MMU() {
    mbc->writeRamToFile(memory);
    mbc.reset();
    rom.reset();
  }

  static const size_t START_OF_ROM = 0x100;

  void printMemory(size_t start, size_t end) {
    if (end > memory.size()) {
      std::cout << memory.size() << "\n";
      std::cerr << "Trying to print out of memory bounds!" << std::endl;
      return;
    }

    for (size_t i = start; i <= end; ++i) {
      if (i % 16 == 0) {
        std::cout << "\n";
      }

      if (i % 16 != 0) {
        std::cout << " ";
      }

      util::printHex(memory[i], 2);
    }
    std::cout << "\n";
  }

  std::vector<uint8_t>& getMemory() { return memory; }
  std::vector<uint8_t>& getBootAreaRemap() { return bootAreaRemap; }

  uint8_t read(uint16_t address, PPU& ppu);
  void write(uint8_t data, uint16_t address, PPU& ppu);
  uint8_t directRead(uint16_t address) {
    if (address == Address::IF_REG_ADDR) {
      return memory[address] | 0xE0;
    }
    return memory[address];
  }
  void directWrite(uint8_t data, uint16_t address) {
    if (address == Address::IF_REG_ADDR) {
      memory[address] = data | 0xE0;
    }
    memory[address] = data;
  }

  void reset() {
    mbc->writeRamToFile(memory);
    for (size_t i = 0; i < BOOT_ROM_SIZE; ++i) {
      memory[i] = BOOT_ROM[i];
    }
    for (size_t i = BOOT_ROM_SIZE; i < MEMORY_SIZE; ++i) {
      memory[i] = rom->at(i);
    }
    mbc->readRamFromFile(memory);
  }

  bool addressInHRAM(uint16_t address) {
    return (address >= 0xFF80 && address <= 0xFFFE);
  }

  bool addressInOAMBugRegion(uint16_t address) {
    return (address >= 0xFE00 && address <= 0xFEFF);
  }
};
