#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace util {
inline std::vector<uint8_t> parseBinFile(std::string filepath) {
  std::ifstream romFile;
  romFile.exceptions(std::ifstream::badbit);

  try {
    romFile.open(filepath, std::ios::binary);
    if (!romFile.good()) {
      return {};
    }
    romFile.unsetf(std::ios::skipws);

    romFile.seekg(0, std::ios::end);
    std::streampos length = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom;
    rom.reserve(length);

    rom.insert(rom.begin(), std::istream_iterator<uint8_t>(romFile),
               std::istream_iterator<uint8_t>());

    romFile.close();

    return rom;
  } catch (const std::ifstream::failure& e) {
    std::cerr << "Exception opening/reading/closing file " << e.what() << "\n";
  }

  return {};
}

inline void writeBinFile(std::string file, std::vector<uint8_t>& bytes) {
  std::ofstream saveFile;

  try {
    saveFile.open(file, std::ios::trunc | std::ios::binary);
    saveFile.write((char*)&bytes[0], bytes.size() * sizeof(uint8_t));
    saveFile.close();
  } catch (const std::ofstream::failure& e) {
    std::cerr << "Exception opening/writingclosing file " << e.what() << "\n";
  }
}
}  // namespace util