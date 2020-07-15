#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "../CPU/Opcodes.hpp"
#include "Logging.hpp"

namespace util {
    int32_t make16Wide(std::vector<uint8_t>& rom, int32_t& i) {
        int16_t low = static_cast<int16_t>(rom[++i]);
        int16_t high = static_cast<int16_t>(rom[++i]);
        int32_t value = low;
        value |= high << 8;

        return value;
    }

    std::pair<std::vector<std::string>, std::unordered_map<uint16_t, int32_t>> disassemble(std::vector<uint8_t> rom) {
        std::vector<std::string> lines;
        std::unordered_map<uint16_t, int32_t> map;
        uint16_t pc = 0;
        lines.reserve(rom.size());

        for (int32_t i = 0; i < rom.size(); ++i) {
            if (i >= 0xA8 && i < 0xE0 || i >= 0x104 && i <= 0x14F || i >= 0x8000 && i <= 0x9FFF) {
                std::string ss = printHexToString(i, 4) + ": db " + printHexToString(rom[i], 2);
                lines.push_back(ss);
                continue;
            }
            std::string ss;
            ss += printHexToString(i, 4);
            ss += ": ";
            pc = i;

            uint8_t currOpVal = rom[i];
            Opcode currOp;
            
            if (currOpVal == 0xCB) {
                currOpVal = rom[++i];
                currOp = OpcodeInfo::CB_OPCODES[currOpVal];
                ss += currOp.mnemonic;
            }
            else {
                currOp = OpcodeInfo::OPCODES[currOpVal];
                ss += currOp.mnemonic;
                if (currOpVal == 0x08) {
                    ss += " ";
                    ss += printHexToString(make16Wide(rom, i), 4);
                    ss += ", SP";
                }
                else if (currOpVal == 0xE0) {
                    uint8_t address = rom[++i];
                    ss += " ";
                    ss += printHexToString(address, 2);
                    ss += ", A";
                }
                else if (currOpVal == 0xEA) {
                    ss += " ";
                    ss += printHexToString(make16Wide(rom, i), 4);
                    ss += ", A";
                }
                else if (currOpVal == 0xF8) {
                    int8_t value = static_cast<int8_t>(rom[++i]);
                    if (value < 0) {
                        ss += std::to_string(static_cast<int16_t>(value));
                    }
                    else {
                        ss += "+";
                        ss += std::to_string(static_cast<int16_t>(value));
                    }
                }
                else if (currOp.length == 2) {
                    if (currOpVal == 0x18 || currOpVal == 0x20 || currOpVal == 0x28 || currOpVal == 0x30
                        || currOpVal == 0x38 || currOpVal == 0xE8) {
                        int8_t value = static_cast<int8_t>(rom[++i]);
                        ss += " ";
                        ss += printHexToString(i + 1 + static_cast<int16_t>(value), 4);
                    }
                    else {
                        uint8_t value = rom[++i];
                        ss += " ";
                        ss += printHexToString(value, 2);
                    }
                }
                else if (currOp.length == 3) {
                    ss += " ";
                    ss += printHexToString(make16Wide(rom, i), 4);
                }
            }

            lines.push_back(ss);
            map[pc] = static_cast<int32_t>(lines.size() - 1);
        }

        return { lines, map };
    }
}