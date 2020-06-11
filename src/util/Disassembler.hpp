#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include "../Opcodes.hpp"

namespace util {
    template <typename T>
    static void printHex(T value, uint32_t width) {
        std::cout << std::setfill('0') << std::setw(width)
            << std::uppercase << std::hex << static_cast<int32_t>(value)
            << std::dec << std::nouppercase << "h";
    }

    static void diassemble(std::vector<uint8_t> rom) {

        std::cout << "\nROM DISASSEMBLY\n===============\n";
        for (int32_t i = 0; i < 258; ++i) {
            printHex(i, 4);
            std::cout << ": ";

            uint8_t currOpVal = rom[i];
            Opcode currOp;
            
            if (currOpVal == 0xCB) {
                currOpVal = rom[++i];
                currOp = OpcodeInfo::CB_OPCODES[currOpVal];
                std::cout << currOp.mnemonic << "\n";
            }
            else {
                currOp = OpcodeInfo::OPCODES[currOpVal];
                std::cout << currOp.mnemonic;
                if (currOpVal == 0x08) {
                    uint16_t address = rom[++i] | (rom[++i] << 8);
                    std::cout << " ";
                    printHex(address, 4);
                    std::cout << ", SP\n";
                }
                else if (currOpVal == 0xE0) {
                    uint8_t address = rom[++i];
                    std::cout << " ";
                    printHex(address, 2);
                    std::cout << ", A\n";
                }
                else if (currOpVal == 0xEA) {
                    int16_t low = static_cast<int16_t>(rom[++i]);
                    int16_t high = static_cast<int16_t>(rom[++i]);
                    int32_t address = low;
                    address |= high << 8;

                    std::cout << " ";
                    printHex(address, 4);
                    std::cout << ", A\n";
                }
                else if (currOpVal == 0xF8) {
                    int8_t value = static_cast<int8_t>(rom[++i]);
                    if (value < 0) {
                        printHex(value, 2);
                        std::cout << "\n";
                    }
                    else {
                        std::cout << "+";
                        printHex(value, 2);
                        std::cout << "\n";
                    }
                }
                else if (currOp.length == 2) {
                    if (currOpVal == 0x18 || currOpVal == 0x20 || currOpVal == 0x28 || currOpVal == 0x30
                        || currOpVal == 0x38 || currOpVal == 0xE8) {
                        int8_t value = static_cast<int8_t>(rom[++i]);
                        std::cout << " ";
                        printHex(i + 1 + static_cast<int16_t>(value), 4);
                        std::cout << "\n";
                    }
                    else {
                        uint8_t value = rom[++i];
                        std::cout << " ";
                        printHex(value, 2);
                        std::cout << "\n";
                    }
                }
                else if (currOp.length == 3) {
                    int16_t low = static_cast<int16_t>(rom[++i]);
                    int16_t high = static_cast<int16_t>(rom[++i]);
                    int32_t value = low;
                    value |= high << 8;

                    std::cout << " ";
                    printHex(value, 4);
                    std::cout << "\n";
                }
                else {
                    std::cout << "\n";
                }
            }
        }

        std::cout << "ROM DISASSEMBLY FINISHED\n========================\n\n";
    }
}