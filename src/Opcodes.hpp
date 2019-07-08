#pragma once
#include <functional>
#include <string>
#include <iostream>
#include "Register.hpp"

struct Opcode
{
    std::string mnemonic;
    uint16_t code;
    uint32_t cycles;
    uint32_t extendedCycles;
    uint16_t length;
    Opcode(std::string m, uint16_t c, uint32_t cy, uint32_t eCy, uint16_t l) : mnemonic(m), code(c), cycles(cy), extendedCycles(eCy), length(l) {}
};    

std::ostream& operator<<(std::ostream& os, const Opcode& op)
{
    os << std::hex << op.code << std::dec << "\n";
    return os;
}

namespace Opcodes
{
    Opcode JP_a16("JP nn", 0xC3, 4, 4, 3);
    Opcode JP_HL("JP HL", 0xE9, 1, 1, 1);
    Opcode JP_NZ_a16("JP NZ, a16", 0xC2, 3, 4, 3);
    Opcode JP_NC_a16("JP NC, a16", 0xD2, 3, 4, 3);
    Opcode JP_Z_a16("JP Z, a16", 0xCA, 3, 4, 3);
    Opcode JP_C_a16("JP C, a16", 0xDA, 3, 4, 3);
    Opcode JR_r8("JR r8", 0x18, 3, 3, 2);
    Opcode JR_NZ_r8("JR NZ, r8", 0x20, 2, 3, 2);
    Opcode JR_NC_r8("JR NC, r8", 0x30, 2, 3, 2);
    Opcode JR_Z_r8("JR Z, r8", 0x28, 2, 3, 2);
    Opcode JR_C_r8("JR C, r8", 0x38, 2, 3, 2);
    Opcode CALL_a16("CALL a16", 0xCD, 6, 6, 3);
    Opcode CALL_NZ_a16("CALL NZ, a16", 0xC4, 3, 6, 3);
    Opcode CALL_NC_a16("CALL NC, a16", 0xD4, 3, 6, 3);
    Opcode CALL_Z_a16("CALL Z, a16", 0xCC, 3, 6, 3);
    Opcode CALL_C_a16("CALL C, a16", 0xDC, 3, 6, 3);
    Opcode RET("RET", 0xC9, 4, 4, 1);
    Opcode RET_NZ("RET NZ", 0xC0, 2, 5, 1);
    Opcode RET_NC("RET NZ", 0xD0, 2, 5, 1);
    Opcode RET_Z("RET Z", 0xC8, 2, 5, 1);
    Opcode RET_C("RET C", 0xD8, 2, 5, 1);

};