#pragma once
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

class ROM {
private:
    std::string file;
    std::vector<uint8_t> rom;

public:
    ROM(std::string f) : file(f) {

        std::ifstream file(f, std::ios::binary);

        // file.unsetf(std::ios::skipws);

        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        rom.reserve(fileSize);

        rom.insert(rom.begin(),
            std::istream_iterator<uint8_t>(file),
            std::istream_iterator<uint8_t>());
    }
};