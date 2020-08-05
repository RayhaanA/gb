#pragma once
#include <array>
#include <inttypes.h>
#include <string>

struct Opcode {
    std::string mnemonic;
    uint8_t cycles;
    uint8_t extendedCycles;
    uint8_t length;
    Opcode() {
        mnemonic = "EMPTY";
        cycles = 0;
        extendedCycles = 0;
        length = 0;
    }
    
    Opcode(std::string m, uint8_t cy, uint8_t eCy, uint8_t l) : mnemonic(m), cycles(cy), extendedCycles(eCy), length(l) {}
};

namespace OpcodeInfo {
    std::array<Opcode, 256> OPCODES = {
        Opcode { "NOP", 1, 1, 1 },
        Opcode { "LD BC,", 3, 3, 3 },
        Opcode { "LD (BC), A", 2, 2, 1 },
        Opcode { "INC BC", 2, 2, 1 },
        Opcode { "INC B", 1, 1, 1 },
        Opcode { "DEC B", 1, 1, 1 },
        Opcode { "LD B,", 2, 2, 2 },
        Opcode { "RLCA", 1, 1, 1 },
        Opcode { "LD", 5, 5, 3 },
        Opcode { "ADD HL, BC", 2, 2, 1 },
        Opcode { "LD A, (BC)", 2, 2, 1 },
        Opcode { "DEC BC", 2, 2, 1 },
        Opcode { "INC C", 1, 1, 1 },
        Opcode { "DEC C", 1, 1, 1 },
        Opcode { "LD C,", 2, 2, 2 },
        Opcode { "RRCA", 1, 1, 1 },
        Opcode { "STOP 0", 1, 1, 2 },
        Opcode { "LD DE,", 3, 3, 3 },
        Opcode { "LD (DE), A", 2, 2, 1 },
        Opcode { "INC DE", 2, 2, 1 },
        Opcode { "INC D", 1, 1, 1 },
        Opcode { "DEC D", 1, 1, 1 },
        Opcode { "LD D,", 2, 2, 2 },
        Opcode { "RLA", 1, 1, 1 },
        Opcode { "JR", 3, 3, 2 },
        Opcode { "ADD HL, DE", 2, 2, 1 },
        Opcode { "LD A, (DE)", 2, 2, 1 },
        Opcode { "DEC DE", 2, 2, 1 },
        Opcode { "INC E", 1, 1, 1 },
        Opcode { "DEC E", 1, 1, 1 },
        Opcode { "LD E,", 2, 2, 2 },
        Opcode { "RRA", 1, 1, 1 },
        Opcode { "JR NZ,", 2, 3, 2 },
        Opcode { "LD HL,", 3, 3, 3 },
        Opcode { "LDI (HL), A", 2, 2, 1 },
        Opcode { "INC HL", 2, 2, 1 },
        Opcode { "INC H", 1, 1, 1 },
        Opcode { "DEC H", 1, 1, 1 },
        Opcode { "LD H,", 2, 2, 2 },
        Opcode { "DAA", 1, 1, 1 },
        Opcode { "JR Z,", 2, 3, 2 },
        Opcode { "ADD HL, HL", 2, 2, 1 },
        Opcode { "LDI A, (HL)", 2, 2, 1 },
        Opcode { "DEC HL", 2, 2, 1 },
        Opcode { "INC L", 1, 1, 1 },
        Opcode { "DEC L", 1, 1, 1 },
        Opcode { "LD L,", 2, 2, 2 },
        Opcode { "CPL", 1, 1, 1 },
        Opcode { "JR NC,", 2, 3, 2 },
        Opcode { "LD SP,", 3, 3, 3 },
        Opcode { "LDD (HL), A", 2, 2, 1 },
        Opcode { "INC SP", 2, 2, 1 },
        Opcode { "INC (HL)", 3, 3, 1 },
        Opcode { "DEC (HL)", 3, 3, 1 },
        Opcode { "LD (HL),", 3, 3, 2 },
        Opcode { "SCF", 1, 1, 1 },
        Opcode { "JR C,", 2, 3, 2 },
        Opcode { "ADD HL, SP", 2, 2, 1 },
        Opcode { "LDD A, (HL)", 2, 2, 1 },
        Opcode { "DEC SP", 2, 2, 1 },
        Opcode { "INC A", 1, 1, 1 },
        Opcode { "DEC A", 1, 1, 1 },
        Opcode { "LD A,", 3, 3, 2 },
        Opcode { "CCF", 1, 1, 1 },
        Opcode { "LD B, B", 1, 1, 1 },
        Opcode { "LD B, C", 1, 1, 1 },
        Opcode { "LD B, D", 1, 1, 1 },
        Opcode { "LD B, E", 1, 1, 1 },
        Opcode { "LD B, H", 1, 1, 1 },
        Opcode { "LD B, L", 1, 1, 1 },
        Opcode { "LD B, (HL)", 2, 2, 1 },
        Opcode { "LD B, A", 1, 1, 1 },
        Opcode { "LD C, B", 1, 1, 1 },
        Opcode { "LD C, C", 1, 1, 1 },
        Opcode { "LD C, D", 1, 1, 1 },
        Opcode { "LD C, E", 1, 1, 1 },
        Opcode { "LD C, H", 1, 1, 1 },
        Opcode { "LD C, L", 1, 1, 1 },
        Opcode { "LD C, (HL)", 2, 2, 1 },
        Opcode { "LD C, A", 1, 1, 1 },
        Opcode { "LD D, B", 1, 1, 1 },
        Opcode { "LD D, C", 1, 1, 1 },
        Opcode { "LD D, D", 1, 1, 1 },
        Opcode { "LD D, E", 1, 1, 1 },
        Opcode { "LD D, H", 1, 1, 1 },
        Opcode { "LD D, L", 1, 1, 1 },
        Opcode { "LD D, (HL)", 2, 2, 1 },
        Opcode { "LD D, A", 1, 1, 1 },
        Opcode { "LD E, B", 1, 1, 1 },
        Opcode { "LD E, C", 1, 1, 1 },
        Opcode { "LD E, D", 1, 1, 1 },
        Opcode { "LD E, E", 1, 1, 1 },
        Opcode { "LD E, H", 1, 1, 1 },
        Opcode { "LD E, L", 1, 1, 1 },
        Opcode { "LD E, (HL)", 2, 2, 1 },
        Opcode { "LD E, A", 1, 1, 1 },
        Opcode { "LD H, B", 1, 1, 1 },
        Opcode { "LD H, C", 1, 1, 1 },
        Opcode { "LD H, D", 1, 1, 1 },
        Opcode { "LD H, E", 1, 1, 1 },
        Opcode { "LD H, H", 1, 1, 1 },
        Opcode { "LD H, L", 1, 1, 1 },
        Opcode { "LD H, (HL)", 2, 2, 1 },
        Opcode { "LD H, A", 1, 1, 1 },
        Opcode { "LD L, B", 1, 1, 1 },
        Opcode { "LD L, C", 1, 1, 1 },
        Opcode { "LD L, E", 1, 1, 1 },
        Opcode { "LD L, D", 1, 1, 1 },
        Opcode { "LD L, H", 1, 1, 1 },
        Opcode { "LD L, L", 1, 1, 1 },
        Opcode { "LD L, (HL)", 2, 2, 1 },
        Opcode { "LD L, A", 1, 1, 1 },
        Opcode { "LD (HL), B", 2, 2, 1 },
        Opcode { "LD (HL), C", 2, 2, 1 },
        Opcode { "LD (HL), D", 2, 2, 1 },
        Opcode { "LD (HL), E", 2, 2, 1 },
        Opcode { "LD (HL), H", 2, 2, 1 },
        Opcode { "LD (HL), L", 2, 2, 1 },
        Opcode { "HALT", 1, 1, 1 },
        Opcode { "LD (HL), A", 2, 2, 1 },
        Opcode { "LD A, B", 1, 1, 1 },
        Opcode { "LD A, C", 1, 1, 1 },
        Opcode { "LD A, D", 1, 1, 1 },
        Opcode { "LD A, E", 1, 1, 1 },
        Opcode { "LD A, H", 1, 1, 1 },
        Opcode { "LD A, L", 1, 1, 1 },
        Opcode { "LD A, (HL)", 2, 2, 1 },
        Opcode { "LD A, A", 1, 1, 1 },
        Opcode { "ADD A, B", 1, 1, 1 },
        Opcode { "ADD A, C", 1, 1, 1 },
        Opcode { "ADD A, D", 1, 1, 1 },
        Opcode { "ADD A, E", 1, 1, 1 },
        Opcode { "ADD A, H", 1, 1, 1 },
        Opcode { "ADD A, L", 1, 1, 1 },
        Opcode { "ADD A, (HL)", 2, 2, 1 },
        Opcode { "ADD A, A", 1, 1, 1 },
        Opcode { "ADC A, B", 1, 1, 1 },
        Opcode { "ADC A, C", 1, 1, 1 },
        Opcode { "ADC A, D", 1, 1, 1 },
        Opcode { "ADC A, E", 1, 1, 1 },
        Opcode { "ADC A, H", 1, 1, 1 },
        Opcode { "ADC A, L", 1, 1, 1 },
        Opcode { "ADC A, (HL)", 2, 2, 1 },
        Opcode { "ADC A, A", 1, 1, 1 },
        Opcode { "SUB B", 1, 1, 1 },
        Opcode { "SUB C", 1, 1, 1 },
        Opcode { "SUB D", 1, 1, 1 },
        Opcode { "SUB E", 1, 1, 1 },
        Opcode { "SUB H", 1, 1, 1 },
        Opcode { "SUB L", 1, 1, 1 },
        Opcode { "SUB (HL)", 2, 2, 1 },
        Opcode { "SUB A", 1, 1, 1 },
        Opcode { "SBC A, B", 1, 1, 1 },
        Opcode { "SBC A, C", 1, 1, 1 },
        Opcode { "SBC A, D", 1, 1, 1 },
        Opcode { "SBC A, E", 1, 1, 1 },
        Opcode { "SBC A, H", 1, 1, 1 },
        Opcode { "SBC A, L", 1, 1, 1 },
        Opcode { "SBC A, (HL)", 2, 2, 1 },
        Opcode { "SBC A, A", 1, 1, 1 },
        Opcode { "AND B", 1, 1, 1 },
        Opcode { "AND C", 1, 1, 1 },
        Opcode { "AND D", 1, 1, 1 },
        Opcode { "AND E", 1, 1, 1 },
        Opcode { "AND H", 1, 1, 1 },
        Opcode { "AND L", 1, 1, 1 },
        Opcode { "AND (HL)", 2, 2, 1 },
        Opcode { "AND A", 1, 1, 1 },
        Opcode { "XOR B", 1, 1, 1 },
        Opcode { "XOR C", 1, 1, 1 },
        Opcode { "XOR D", 1, 1, 1 },
        Opcode { "XOR E", 1, 1, 1 },
        Opcode { "XOR H", 1, 1, 1 },
        Opcode { "XOR L", 1, 1, 1 },
        Opcode { "XOR (HL)", 2, 2, 1 },
        Opcode { "XOR A", 1, 1, 1 },
        Opcode { "OR B", 1, 1, 1 },
        Opcode { "OR C", 1, 1, 1 },
        Opcode { "OR D", 1, 1, 1 },
        Opcode { "OR E", 1, 1, 1 },
        Opcode { "OR H", 1, 1, 1 },
        Opcode { "OR L", 1, 1, 1 },
        Opcode { "OR (HL)", 2, 2, 1 },
        Opcode { "OR A", 1, 1, 1 },
        Opcode { "CP B", 1, 1, 1 },
        Opcode { "CP C", 1, 1, 1 },
        Opcode { "CP D", 1, 1, 1 },
        Opcode { "CP E", 1, 1, 1 },
        Opcode { "CP H", 1, 1, 1 },
        Opcode { "CP L", 1, 1, 1 },
        Opcode { "CP (HL)", 2, 2, 1 },
        Opcode { "CP A", 1, 1, 1 },
        Opcode { "RET NZ", 2, 5, 1 },
        Opcode { "POP BC", 3, 3, 1 },
        Opcode { "JP NZ,", 3, 4, 3 },
        Opcode { "JP", 4, 4, 3 },
        Opcode { "CALL NZ,", 3, 6, 3 },
        Opcode { "PUSH BC", 4, 4, 1 },
        Opcode { "ADD A,", 2, 2, 2 },
        Opcode { "RST 00H", 4, 4, 1 },
        Opcode { "RET Z", 2, 5, 1 },
        Opcode { "RET", 4, 4, 1 },
        Opcode { "JP Z,", 3, 4, 3 },
        Opcode { "PREFIX CB", 1, 1, 1 },
        Opcode { "CALL Z,", 3, 6, 3 },
        Opcode { "CALL", 6, 6, 3 },
        Opcode { "ADC A,", 2, 2, 2 },
        Opcode { "RST 08H", 4, 4, 1 },
        Opcode { "RET NC", 2, 5, 1 },
        Opcode { "POP DE", 3, 3, 1 },
        Opcode { "JP NC,", 3, 4, 3 },
        Opcode {},
        Opcode { "CALL NC,", 3, 6, 3 },
        Opcode { "PUSH DE", 4, 4, 1 },
        Opcode { "SUB", 2, 2, 2 },
        Opcode { "RST 10H", 4, 4, 1 },
        Opcode { "RET C", 2, 5, 1 },
        Opcode { "RETI", 4, 4, 1 },
        Opcode { "JP C,", 3, 4, 3 },
        Opcode {},
        Opcode { "CALL C,", 3, 6, 3 },
        Opcode {},
        Opcode { "SBC A,", 2, 2, 2 },
        Opcode { "RST 18H", 4, 4, 1 },
        Opcode { "LDH", 3, 3, 2 },
        Opcode { "POP HL", 3, 3, 1 },
        Opcode { "LD (C), A", 2, 2, 1 },
        Opcode {},
        Opcode {},
        Opcode { "PUSH HL", 4, 4, 1 },
        Opcode { "AND", 2, 2, 2 },
        Opcode { "RST 20H", 4, 4, 1 },
        Opcode { "ADD SP,", 4, 4, 2 },
        Opcode { "JP (HL)", 1, 1, 1 },
        Opcode { "LD", 4, 4, 3 },
        Opcode {},
        Opcode {},
        Opcode {},
        Opcode { "XOR", 2, 2, 2 },
        Opcode { "RST 28H", 4, 4, 1 },
        Opcode { "LDH A,", 3, 3, 2 },
        Opcode { "POP AF", 3, 3, 1 },
        Opcode { "LD A, (C)", 2, 2, 1 },
        Opcode { "DI", 1, 1, 1 },
        Opcode {},
        Opcode { "PUSH AF", 4, 4, 1 },
        Opcode { "OR", 2, 2, 2 },
        Opcode { "RST 30H", 4, 4, 1 },
        Opcode { "LD HL, SP", 3, 3, 2 },
        Opcode { "LD SP, HL", 2, 2, 1 },
        Opcode { "LD A,", 4, 4, 3 },
        Opcode { "EI", 1, 2, 1 },
        Opcode {},
        Opcode {},
        Opcode { "CP", 2, 2, 2 },
        Opcode { "RST 38H", 4, 4, 1 }
    };

    std::array<Opcode, 256> CB_OPCODES = {
        Opcode { "RLC B", 2, 2, 2 },
        Opcode { "RLC C", 2, 2, 2 },
        Opcode { "RLC D", 2, 2, 2 },
        Opcode { "RLC E", 2, 2, 2 },
        Opcode { "RLC H", 2, 2, 2 },
        Opcode { "RLC L", 2, 2, 2 },
        Opcode { "RLC (HL)", 4, 4, 2 },
        Opcode { "RLC A", 2, 2, 2 },
        Opcode { "RRC B", 2, 2, 2 },
        Opcode { "RRC C", 2, 2, 2 },
        Opcode { "RRC D", 2, 2, 2 },
        Opcode { "RRC E", 2, 2, 2 },
        Opcode { "RRC H", 2, 2, 2 },
        Opcode { "RRC L", 2, 2, 2 },
        Opcode { "RRC (HL)", 4, 4, 2 },
        Opcode { "RRC A", 2, 2, 2 },
        Opcode { "RL B", 2, 2, 2 },
        Opcode { "RL C", 2, 2, 2 },
        Opcode { "RL D", 2, 2, 2 },
        Opcode { "RL E", 2, 2, 2 },
        Opcode { "RL H", 2, 2, 2 },
        Opcode { "RL L", 2, 2, 2 },
        Opcode { "RL (HL)", 4, 4, 2 },
        Opcode { "RL A", 2, 2, 2 },
        Opcode { "RR B", 2, 2, 2 },
        Opcode { "RR C", 2, 2, 2 },
        Opcode { "RR D", 2, 2, 2 },
        Opcode { "RR E", 2, 2, 2 },
        Opcode { "RR H", 2, 2, 2 },
        Opcode { "RR L", 2, 2, 2 },
        Opcode { "RR (HL)", 4, 4, 2 },
        Opcode { "RR A", 2, 2, 2 },
        Opcode { "SLA B", 2, 2, 2 },
        Opcode { "SLA C", 2, 2, 2 },
        Opcode { "SLA D", 2, 2, 2 },
        Opcode { "SLA E", 2, 2, 2 },
        Opcode { "SLA H", 2, 2, 2 },
        Opcode { "SLA L", 2, 2, 2 },
        Opcode { "SLA (HL)", 4, 4, 2 },
        Opcode { "SLA A", 2, 2, 2 },
        Opcode { "SRA B", 2, 2, 2 },
        Opcode { "SRA C", 2, 2, 2 },
        Opcode { "SRA D", 2, 2, 2 },
        Opcode { "SRA E", 2, 2, 2 },
        Opcode { "SRA H", 2, 2, 2 },
        Opcode { "SRA L", 2, 2, 2 },
        Opcode { "SRA (HL)", 4, 4, 2 },
        Opcode { "SRA A", 2, 2, 2 },
        Opcode { "SWAP B", 2, 2, 2 },
        Opcode { "SWAP C", 2, 2, 2 },
        Opcode { "SWAP D", 2, 2, 2 },
        Opcode { "SWAP E", 2, 2, 2 },
        Opcode { "SWAP H", 2, 2, 2 },
        Opcode { "SWAP L", 2, 2, 2 },
        Opcode { "SWAP (HL)", 4, 4, 2 },
        Opcode { "SWAP A", 2, 2, 2 },
        Opcode { "SRL B", 2, 2, 2 },
        Opcode { "SRL C", 2, 2, 2 },
        Opcode { "SRL D", 2, 2, 2 },
        Opcode { "SRL E", 2, 2, 2 },
        Opcode { "SRL H", 2, 2, 2 },
        Opcode { "SRL L", 2, 2, 2 },
        Opcode { "SRL (HL)", 4, 4, 2 },
        Opcode { "SRL A", 2, 2, 2 },
        Opcode { "BIT 0, B", 2, 2, 2 },
        Opcode { "BIT 0, C", 2, 2, 2 },
        Opcode { "BIT 0, D", 2, 2, 2 },
        Opcode { "BIT 0, E", 2, 2, 2 },
        Opcode { "BIT 0, H", 2, 2, 2 },
        Opcode { "BIT 0, L", 2, 2, 2 },
        Opcode { "BIT 0, (HL)", 4, 4, 2 },
        Opcode { "BIT 0, A", 2, 2, 2 },
        Opcode { "BIT 1, B", 2, 2, 2 },
        Opcode { "BIT 1, C", 2, 2, 2 },
        Opcode { "BIT 1, D", 2, 2, 2 },
        Opcode { "BIT 1, E", 2, 2, 2 },
        Opcode { "BIT 1, H", 2, 2, 2 },
        Opcode { "BIT 1, L", 2, 2, 2 },
        Opcode { "BIT 1, (HL)", 4, 4, 2 },
        Opcode { "BIT 1, A", 2, 2, 2 },
        Opcode { "BIT 2, B", 2, 2, 2 },
        Opcode { "BIT 2, C", 2, 2, 2 },
        Opcode { "BIT 2, D", 2, 2, 2 },
        Opcode { "BIT 2, E", 2, 2, 2 },
        Opcode { "BIT 2, H", 2, 2, 2 },
        Opcode { "BIT 2, L", 2, 2, 2 },
        Opcode { "BIT 2, (HL)", 4, 4, 2 },
        Opcode { "BIT 2, A", 2, 2, 2 },
        Opcode { "BIT 3, B", 2, 2, 2 },
        Opcode { "BIT 3, C", 2, 2, 2 },
        Opcode { "BIT 3, D", 2, 2, 2 },
        Opcode { "BIT 3, E", 2, 2, 2 },
        Opcode { "BIT 3, H", 2, 2, 2 },
        Opcode { "BIT 3, L", 2, 2, 2 },
        Opcode { "BIT 3, (HL)", 4, 4, 2 },
        Opcode { "BIT 3, A", 2, 2, 2 },
        Opcode { "BIT 4, B", 2, 2, 2 },
        Opcode { "BIT 4, C", 2, 2, 2 },
        Opcode { "BIT 4, D", 2, 2, 2 },
        Opcode { "BIT 4, E", 2, 2, 2 },
        Opcode { "BIT 4, H", 2, 2, 2 },
        Opcode { "BIT 4, L", 2, 2, 2 },
        Opcode { "BIT 4, (HL)", 4, 4, 2 },
        Opcode { "BIT 4, A", 2, 2, 2 },
        Opcode { "BIT 5, B", 2, 2, 2 },
        Opcode { "BIT 5, C", 2, 2, 2 },
        Opcode { "BIT 5, D", 2, 2, 2 },
        Opcode { "BIT 5, E", 2, 2, 2 },
        Opcode { "BIT 5, H", 2, 2, 2 },
        Opcode { "BIT 5, L", 2, 2, 2 },
        Opcode { "BIT 5, (HL)", 4, 4, 2 },
        Opcode { "BIT 5, A", 2, 2, 2 },
        Opcode { "BIT 6, B", 2, 2, 2 },
        Opcode { "BIT 6, C", 2, 2, 2 },
        Opcode { "BIT 6, D", 2, 2, 2 },
        Opcode { "BIT 6, E", 2, 2, 2 },
        Opcode { "BIT 6, H", 2, 2, 2 },
        Opcode { "BIT 6, L", 2, 2, 2 },
        Opcode { "BIT 6, (HL)", 4, 4, 2 },
        Opcode { "BIT 6, A", 2, 2, 2 },
        Opcode { "BIT 7, B", 2, 2, 2 },
        Opcode { "BIT 7, C", 2, 2, 2 },
        Opcode { "BIT 7, D", 2, 2, 2 },
        Opcode { "BIT 7, E", 2, 2, 2 },
        Opcode { "BIT 7, H", 2, 2, 2 },
        Opcode { "BIT 7, L", 2, 2, 2 },
        Opcode { "BIT 7, (HL)", 4, 4, 2 },
        Opcode { "BIT 7, A", 2, 2, 2 },
        Opcode { "RES 0, B", 2, 2, 2 },
        Opcode { "RES 0, C", 2, 2, 2 },
        Opcode { "RES 0, D", 2, 2, 2 },
        Opcode { "RES 0, E", 2, 2, 2 },
        Opcode { "RES 0, H", 2, 2, 2 },
        Opcode { "RES 0, L", 2, 2, 2 },
        Opcode { "RES 0, (HL)", 4, 4, 2 },
        Opcode { "RES 0, A", 2, 2, 2 },
        Opcode { "RES 1, B", 2, 2, 2 },
        Opcode { "RES 1, C", 2, 2, 2 },
        Opcode { "RES 1, D", 2, 2, 2 },
        Opcode { "RES 1, E", 2, 2, 2 },
        Opcode { "RES 1, H", 2, 2, 2 },
        Opcode { "RES 1, L", 2, 2, 2 },
        Opcode { "RES 1, (HL)", 4, 4, 2 },
        Opcode { "RES 1, A", 2, 2, 2 },
        Opcode { "RES 2, B", 2, 2, 2 },
        Opcode { "RES 2, C", 2, 2, 2 },
        Opcode { "RES 2, D", 2, 2, 2 },
        Opcode { "RES 2, E", 2, 2, 2 },
        Opcode { "RES 2, H", 2, 2, 2 },
        Opcode { "RES 2, L", 2, 2, 2 },
        Opcode { "RES 2, (HL)", 4, 4, 2 },
        Opcode { "RES 2, A", 2, 2, 2 },
        Opcode { "RES 3, B", 2, 2, 2 },
        Opcode { "RES 3, C", 2, 2, 2 },
        Opcode { "RES 3, D", 2, 2, 2 },
        Opcode { "RES 3, E", 2, 2, 2 },
        Opcode { "RES 3, H", 2, 2, 2 },
        Opcode { "RES 3, L", 2, 2, 2 },
        Opcode { "RES 3, (HL)", 4, 4, 2 },
        Opcode { "RES 3, A", 2, 2, 2 },
        Opcode { "RES 4, B", 2, 2, 2 },
        Opcode { "RES 4, C", 2, 2, 2 },
        Opcode { "RES 4, D", 2, 2, 2 },
        Opcode { "RES 4, E", 2, 2, 2 },
        Opcode { "RES 4, H", 2, 2, 2 },
        Opcode { "RES 4, L", 2, 2, 2 },
        Opcode { "RES 4, (HL)", 4, 4, 2 },
        Opcode { "RES 4, A", 2, 2, 2 },
        Opcode { "RES 5, B", 2, 2, 2 },
        Opcode { "RES 5, C", 2, 2, 2 },
        Opcode { "RES 5, D", 2, 2, 2 },
        Opcode { "RES 5, E", 2, 2, 2 },
        Opcode { "RES 5, H", 2, 2, 2 },
        Opcode { "RES 5, L", 2, 2, 2 },
        Opcode { "RES 5, (HL)", 4, 4, 2 },
        Opcode { "RES 5, A", 2, 2, 2 },
        Opcode { "RES 6, B", 2, 2, 2 },
        Opcode { "RES 6, C", 2, 2, 2 },
        Opcode { "RES 6, D", 2, 2, 2 },
        Opcode { "RES 6, E", 2, 2, 2 },
        Opcode { "RES 6, H", 2, 2, 2 },
        Opcode { "RES 6, L", 2, 2, 2 },
        Opcode { "RES 6, (HL)", 4, 4, 2 },
        Opcode { "RES 6, A", 2, 2, 2 },
        Opcode { "RES 7, B", 2, 2, 2 },
        Opcode { "RES 7, C", 2, 2, 2 },
        Opcode { "RES 7, D", 2, 2, 2 },
        Opcode { "RES 7, E", 2, 2, 2 },
        Opcode { "RES 7, H", 2, 2, 2 },
        Opcode { "RES 7, L", 2, 2, 2 },
        Opcode { "RES 7, (HL)", 4, 4, 2 },
        Opcode { "RES 7, A", 2, 2, 2 },
        Opcode { "SET 0, B", 2, 2, 2 },
        Opcode { "SET 0, C", 2, 2, 2 },
        Opcode { "SET 0, D", 2, 2, 2 },
        Opcode { "SET 0, E", 2, 2, 2 },
        Opcode { "SET 0, H", 2, 2, 2 },
        Opcode { "SET 0, L", 2, 2, 2 },
        Opcode { "SET 0, (HL)", 4, 4, 2 },
        Opcode { "SET 0, A", 2, 2, 2 },
        Opcode { "SET 1, B", 2, 2, 2 },
        Opcode { "SET 1, C", 2, 2, 2 },
        Opcode { "SET 1, D", 2, 2, 2 },
        Opcode { "SET 1, E", 2, 2, 2 },
        Opcode { "SET 1, H", 2, 2, 2 },
        Opcode { "SET 1, L", 2, 2, 2 },
        Opcode { "SET 1, (HL)", 4, 4, 2 },
        Opcode { "SET 1, A", 2, 2, 2 },
        Opcode { "SET 2, B", 2, 2, 2 },
        Opcode { "SET 2, C", 2, 2, 2 },
        Opcode { "SET 2, D", 2, 2, 2 },
        Opcode { "SET 2, E", 2, 2, 2 },
        Opcode { "SET 2, H", 2, 2, 2 },
        Opcode { "SET 2, L", 2, 2, 2 },
        Opcode { "SET 2, (HL)", 4, 4, 2 },
        Opcode { "SET 2, A", 2, 2, 2 },
        Opcode { "SET 3, B", 2, 2, 2 },
        Opcode { "SET 3, C", 2, 2, 2 },
        Opcode { "SET 3, D", 2, 2, 2 },
        Opcode { "SET 3, E", 2, 2, 2 },
        Opcode { "SET 3, H", 2, 2, 2 },
        Opcode { "SET 3, L", 2, 2, 2 },
        Opcode { "SET 3, (HL)", 4, 4, 2 },
        Opcode { "SET 3, A", 2, 2, 2 },
        Opcode { "SET 4, B", 2, 2, 2 },
        Opcode { "SET 4, C", 2, 2, 2 },
        Opcode { "SET 4, D", 2, 2, 2 },
        Opcode { "SET 4, E", 2, 2, 2 },
        Opcode { "SET 4, H", 2, 2, 2 },
        Opcode { "SET 4, L", 2, 2, 2 },
        Opcode { "SET 4, (HL)", 4, 4, 2 },
        Opcode { "SET 4, A", 2, 2, 2 },
        Opcode { "SET 5, B", 2, 2, 2 },
        Opcode { "SET 5, C", 2, 2, 2 },
        Opcode { "SET 5, D", 2, 2, 2 },
        Opcode { "SET 5, E", 2, 2, 2 },
        Opcode { "SET 5, H", 2, 2, 2 },
        Opcode { "SET 5, L", 2, 2, 2 },
        Opcode { "SET 5, (HL)", 4, 4, 2 },
        Opcode { "SET 5, A", 2, 2, 2 },
        Opcode { "SET 6, B", 2, 2, 2 },
        Opcode { "SET 6, C", 2, 2, 2 },
        Opcode { "SET 6, D", 2, 2, 2 },
        Opcode { "SET 6, E", 2, 2, 2 },
        Opcode { "SET 6, H", 2, 2, 2 },
        Opcode { "SET 6, L", 2, 2, 2 },
        Opcode { "SET 6, (HL)", 4, 4, 2 },
        Opcode { "SET 6, A", 2, 2, 2 },
        Opcode { "SET 7, B", 2, 2, 2 },
        Opcode { "SET 7, C", 2, 2, 2 },
        Opcode { "SET 7, D", 2, 2, 2 },
        Opcode { "SET 7, E", 2, 2, 2 },
        Opcode { "SET 7, H", 2, 2, 2 },
        Opcode { "SET 7, L", 2, 2, 2 },
        Opcode { "SET 7, (HL)", 4, 4, 2 },
        Opcode { "SET 7, A", 2, 2, 2 }
    };
}