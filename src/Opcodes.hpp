#pragma once
#include <functional>
#include <string>
#include <iostream>
#include "Register.hpp"

struct Opcode
{
    std::string mnemonic;
    uint16_t code;
};    

std::ostream& operator<<(std::ostream& os, const Opcode& op)
{
    os << std::hex << op.code << std::dec << "\n";
    return os;
}

class Opcodes
{

};