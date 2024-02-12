#pragma once
#include <vector>

#include "../../PPU/PPU.hpp"
#include "../../util/ParseBinFile.hpp"

class MemoryController {
 protected:
  std::shared_ptr<std::vector<uint8_t>> rom;
  size_t romSize = 0;
  uint16_t numRomBanks = 0;
  size_t ramSize = 0;
  uint16_t numRamBanks = 0;
  uint16_t romBank = 1;
  uint16_t ramBank = 0;
  const size_t BASE_ROM_SIZE = 32_KB;
  uint16_t romBankMask = 0;
  bool hasBattery = false;
  std::string ramFile;

  std::string getSavFileForGbFile() {
    return ramFile.substr(0, ramFile.length() - 3) + ".sav";
  }

 public:
  explicit MemoryController(std::vector<uint8_t>* r, uint8_t romInfo,
                            uint8_t ramInfo, bool battery, std::string file) {
    hasBattery = battery;
    ramFile = file;
    rom = std::shared_ptr<std::vector<uint8_t>>(r);
    romSize = BASE_ROM_SIZE << romInfo;
    numRomBanks = 2 << romInfo;
    while (romBankMask < numRomBanks) {
      romBankMask = (romBankMask << 1) + 1;
    }

    switch (ramInfo) {
      case 0x1:
        ramSize = 2_KB;
        break;
      case 0x2:
        ramSize = 8_KB;
        numRamBanks = 1;
        break;
      case 0x3:
        ramSize = 32_KB;
        numRamBanks = 4;
        break;
      case 0x4:
        ramSize = 128_KB;
        numRamBanks = 16;
        break;
      case 0x5:
        ramSize = 64_KB;
        numRamBanks = 8;
        break;
    }
    std::cout << "Rom size " << romSize << "\nRam size " << ramSize << "\n";
  }
  ~MemoryController() { rom.reset(); }
  virtual uint8_t read(std::vector<uint8_t>& memory, uint16_t address,
                       PPU& ppu) = 0;
  virtual void write(std::vector<uint8_t>& memory, uint8_t data,
                     uint16_t address, PPU& ppu) = 0;

  void readRamFromFile(std::vector<uint8_t>& memory) {
    if (!hasBattery) return;
    std::cout << "Reading saved RAM from file\n";
    std::string ramFile = getSavFileForGbFile();
    std::vector<uint8_t> savedRam = util::parseBinFile(ramFile);
    if (savedRam.size() < 8_KB) {
      return;
    }

    for (int i = 0; i < savedRam.size(); ++i) {
      if (i >= 16_KB) {
        break;
      }
      memory[0xA000 + i] = savedRam[i];
    }
  }

  void writeRamToFile(std::vector<uint8_t>& memory) {
    if (!hasBattery) return;
    std::cout << "Writing current RAM from file\n";
    std::vector<uint8_t>::const_iterator first = memory.begin() + 0xA000;
    std::vector<uint8_t>::const_iterator last = memory.begin() + 0xE000;

    std::vector<uint8_t> currentRam(first, last);

    std::string ramFile = getSavFileForGbFile();
    util::writeBinFile(ramFile, currentRam);
  }
};