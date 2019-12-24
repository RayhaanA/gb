#pragma once
#include "Registers.hpp"
#include "MMU.hpp"
#include <iostream>

constexpr uint8_t Z_FLAG = 1 << 7;
constexpr uint8_t N_FLAG = 1 << 6;
constexpr uint8_t H_FLAG = 1 << 5;
constexpr uint8_t C_FLAG = 1 << 4;


class CPU {
private:
    Register A, B, C, D, E, F, H, L;
    RegisterPair AF{ &F, &A };
    RegisterPair BC{ &C, &B };
    RegisterPair DE{ &E, &D };
    RegisterPair HL{ &L, &H };
    SpecialRegister PC, SP;
    bool IME = false; // Interrupt Master Enable Flag: Set through op codes
    bool stopped = false;
    MMU memory;

    // Functions
    uint16_t makeU16(uint8_t lsb, uint8_t msb);
    uint8_t readU8();
    int8_t readS8();
    uint16_t readU16();
    void setFlags(uint8_t value);
    void unsetFlags(uint8_t value);
    bool checkFlags(uint8_t value);

    void addByteValueToA(uint8_t data);
    void addTwoByteValueToRegisterPair(RegisterPair& dest, uint16_t data);
    void subByteValueFromA(uint8_t data);
    void andByteValueAgainstA(uint8_t data);
    void orByteValueAgainstA(uint8_t data);
    void xorByteValueAgainstA(uint8_t data);
    void cpByteValueAgainstA(uint8_t data);
    void adcByteValueAgainstA(uint8_t data);
    void sbcByteValueAgainstA(uint8_t data);
    
    void NOP();
    void STOP();
    void HALT();
    void JP(uint16_t address);
    void JP(bool condition, uint16_t address);
    void JR(int8_t offset);
    void JR(bool condition, int8_t offset);
    void CALL(uint16_t address);
    void CALL(bool condition, uint16_t address);
    void RET();
    void RET(bool condition);
    void RETI();
    void RST(uint8_t lsb);
    void DI();
    void EI();
    void CCF();
    void SCF();
    void DAA();
    void CPL();
    void LD(Register& dest, Register src);
    void LD(Register& dest, uint16_t address);
    void LD(RegisterPair address, Register src);
    void LD(Register& dest, uint8_t value);
    void LD(RegisterPair& dest, uint16_t value);
    void LD(Register& dest, RegisterPair address);
    void LD(SpecialRegister&dest, RegisterPair src);
    void LD(SpecialRegister& dest, uint16_t value);
    void LD(uint16_t address, SpecialRegister src);
    void LD(uint16_t address, Register src);
    void LD(RegisterPair& dest, SpecialRegister src, int8_t offset);
    void LDI(Register src, bool setA);
    void LDD(Register src, bool setA);
    void INC(Register& r);
    void INC(RegisterPair& r);
    void INC(SpecialRegister& r);
    void INC(uint16_t address);
    void DEC(Register& r);
    void DEC(RegisterPair& r);
    void DEC(SpecialRegister& r);
    void DEC(uint16_t address);
    void ADD(Register src);
    void ADD(uint8_t data);
    void ADD(uint16_t address);
    void ADD(RegisterPair& dest, RegisterPair src);
    void ADD(SpecialRegister& dest, int8_t data);
    void ADD(RegisterPair& dest, SpecialRegister src);
    void SUB(Register src);
    void SUB(uint16_t address);
    void SUB(uint8_t data);
    void AND(Register src);
    void AND(uint16_t adddress);
    void AND(uint8_t data);
    void OR(Register src);
    void OR(uint16_t address);
    void OR(uint8_t data);
    void XOR(Register src);
    void XOR(uint16_t address);
    void XOR(uint8_t data);
    void CP(Register src);
    void CP(uint16_t address);
    void CP(uint8_t data);
    void ADC(Register src);
    void ADC(uint16_t address);
    void ADC(uint8_t data);
    void SBC(Register src);
    void SBC(uint16_t address);
    void SBC(uint8_t data);
    void POP(RegisterPair& dest);
    void PUSH(RegisterPair& dest);
    void RLA();
    void RLCA();
    void RRA();
    void RRCA();
    void RLC(Register& r);
    void RLC(uint16_t address);
    void RRC(Register& r);
    void RRC(uint16_t address);
    void RL(Register& r);
    void RL(uint16_t address);
    void RR(Register& r);
    void RR(uint16_t address);
    void SLA(Register& r);
    void SLA(uint16_t address);
    void SRA(Register& r);
    void SRA(uint16_t address);
    void SWAP(Register& r);
    void SWAP(uint16_t address);
    void SRL(Register& r);
    void SRL(uint16_t address);
    void BIT(uint8_t position, Register src);
    void BIT(uint8_t position, uint16_t address);
    void RES(uint8_t position, Register src);
    void RES(uint8_t position, uint16_t address);
    void SET(uint8_t position, Register src);
    void SET(uint8_t position, uint16_t address);
public:
    CPU(MMU mmu) : memory(mmu) {}
    ~CPU() {}

    void callInstruction(uint16_t instruction);
    void callInstructionCB(uint16_t instruction);
    void tick();
    uint16_t getPC() const { return PC.data; }
    // For debugging
    void printMemory(size_t start, size_t end) {
        memory.printMemory(start, end);
    }

    void printRegisters() {
        std::cout << "A = " << A << "B = " << B << "C = " << C << "D = " << D << "E = " << E
           << "F = " << F << "H = " << H << "L = " << L  << "BC = " << BC 
            << "DE = " << DE << "AF = " << AF << "HL = " << HL << "SP = " << SP
           << "PC = " << PC;
     }
};
