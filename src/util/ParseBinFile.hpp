#pragma once
#include <vector>
#include <fstream>
#include <iostream>

namespace util {
    std::vector<uint8_t> parseRomFile(std::string filepath) {
        std::ifstream romFile;
        romFile.exceptions(std::ifstream::badbit);

        try {
            romFile.open(filepath, std::ios::binary);
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
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "Exception opening/reading/closing file " << e.what() << "\n";
        }

        return {};
    }
}