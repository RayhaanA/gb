#pragma once
#include "Registers.hpp"
#include "../PPU/PPU.hpp"
#include "../MMU/MMU.hpp"
#include "Timers.hpp"
#include <iostream>

class CPU {
private:
    Register A, B, C, D, E, F, H, L;
    RegisterPair AF{ &F, &A };
    RegisterPair BC{ &C, &B };
    RegisterPair DE{ &E, &D };
    RegisterPair HL{ &L, &H };
    uint8_t pA, pF, pB, pC, pD, pE, pH, pL, pIF, pIE;
    uint16_t pPC, pSP, pAF, pBC, pDE, pHL;
    SpecialRegister PC, SP;

    bool interruptHandledThisTick = false;
    uint32_t cycles = 0;
    uint32_t frameCycles = 0;
    bool frameDone = false;
    std::unique_ptr<MMU> memory;
    std::unique_ptr<PPU> ppu;
    std::unique_ptr<Timers> timers;

    uint16_t currInstruction = 0;

    // Functions
    uint16_t makeU16(uint8_t lsb, uint8_t msb);
    uint8_t readU8();
    int8_t readS8();
    uint16_t readU16();
    void setFlags(uint8_t value);
    void unsetFlags(uint8_t value);
    bool checkFlags(uint8_t value);
    void requestInterrupt(uint8_t data);
    void clearInterruptRequest(uint8_t data);

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
    void JP_HL();
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
    void LD(SpecialRegister& dest, RegisterPair src);
    void LD(SpecialRegister& dest, uint16_t value);
    void LD(uint16_t address, SpecialRegister src);
    void LD(uint16_t address, Register src);
    void LD(uint16_t address, uint8_t data);
    void LD(RegisterPair& dest, SpecialRegister src, int8_t offset);
    void LDI(Register& src, bool setA);
    void LDD(Register& src, bool setA);
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
    void BIT(uint8_t position, Register& src);
    void BIT(uint8_t position, uint16_t address);
    void RES(uint8_t position, Register& src);
    void RES(uint8_t position, uint16_t address);
    void SET(uint8_t position, Register& src);
    void SET(uint8_t position, uint16_t address);
   
    inline void updatePreviousRegisterValues() {
        pA = A.data;
        pF = F.data;
        pB = B.data;
        pC = C.data;
        pD = D.data;
        pE = E.data;
        pH = H.data;
        pL = L.data;
        pIF = getIF();
        pIE = getIE();
        pPC = PC.data;
        pSP = SP.data;
        pAF = AF.getData();
        pBC = BC.getData();
        pDE = DE.getData();
        pHL = HL.getData();
    }

    bool haltBugActive = false; // System encounters a bug when halt mode is entered, IME = 0, and an Interrupt is ready and enabled
                                // This affects the system by the next instruction not incrementing the PC
    void handleInterrupts();
    bool checkForInterrupts();

    // Increment cycles, check for interrupt enabling, increment timers, check for active DMA, forward time change to PPU
    void incrementCycleCount();

    uint8_t readMemoryAndIncrementCycles(uint16_t address) {
        // Can only access HRAM during DMA
        if (dmaActive && !memory->addressInHRAM(address)) {
            return UNDEFINED_READ;
        }

        uint8_t data = memory->read(address, *ppu);
        incrementCycleCount();
        return data;
    }

    void writeMemoryAndIncrementCycles(uint8_t data, uint16_t address) {
        // Can only access HRAM during DMA
        if (dmaActive && !memory->addressInHRAM(address)) {
            return;
        }
        memory->write(data, address, *ppu);
        incrementCycleCount();
    }

    void dmaCopyByte() {
        if (numBytesCopiedDuringDMA < 159) {
            getMemory()[OAM_TABLE_ADDR + numBytesCopiedDuringDMA] = getMemory()[dmaSourceAddress + numBytesCopiedDuringDMA];
            ++numBytesCopiedDuringDMA;
        }
        else {
            getMemory()[OAM_TABLE_ADDR + numBytesCopiedDuringDMA] = getMemory()[dmaSourceAddress + numBytesCopiedDuringDMA];
            dmaActive = false;
            numBytesCopiedDuringDMA = 0;
        }
    }

    // Bug that happens when doing stuff in 0xFE00 - 0xFEFF range during mode 2 of PPU
    bool oamBugShouldOccur(uint16_t address) { return ppu->getMode() == PPUMode::OAM_SEARCH && memory->addressInOAMBugRegion(address) && ppu->getDisplayEnabled(); }
    void triggerOAMBug(bool read);
    void triggerOAMBugIncrease();
    uint8_t oamBugWrite(uint8_t a, uint8_t b, uint8_t c) { return ((a ^ c) & (b ^ c)) ^ c; }
    uint8_t oamBugRead(uint8_t a, uint8_t b, uint8_t c) { return b | (a & c); }
    uint8_t oamBugReadIncrease(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { return (b & (a | c | d)) | (a & c & d); }

public:
    CPU() = default;
    explicit CPU(MMU* mmu, PPU* p, Timers* t) {
        memory = std::unique_ptr<MMU>(mmu);
        ppu = std::unique_ptr<PPU>(p);
        timers = std::unique_ptr<Timers>(t);
        pA = pF = pB = pC = pD = pE = pH = pL = 0;
        pPC = pSP = pAF =pBC = pDE = pHL = 0;
    }
    ~CPU() {
        ppu.release();
        memory.release();
        timers.release();
    }

    void callInstruction(uint16_t instruction);
    void callInstructionCB(uint16_t instruction);

    void tick();

    std::vector<uint8_t>& getMemory()  { return memory->getMemory(); }
    uint32_t getCurrentCycleCount() const { return cycles; }

    uint8_t getA() const { return A.data; }
    uint8_t getF() const { return F.data; }
    uint8_t getB() const { return B.data; }
    uint8_t getC() const { return C.data; }
    uint8_t getD() const { return D.data; }
    uint8_t getE() const { return E.data; }
    uint8_t getH() const { return H.data; }
    uint8_t getL() const { return L.data; }
    uint16_t getAF() const { return AF.getData(); }
    uint16_t getBC() const { return BC.getData(); }
    uint16_t getDE() const { return DE.getData(); }
    uint16_t getHL() const { return HL.getData(); }
    uint16_t getSP() const { return SP.data; }
    uint16_t getPC() const { return PC.data; }
    uint8_t getIF() { return memory->directRead(IF_REG_ADDR); }
    uint8_t getIE() { return memory->directRead(IE_REG_ADDR); }
    uint8_t getAPrev() const { return pA; }
    uint8_t getFPrev() const { return pF; }
    uint8_t getBPrev() const { return pB; }
    uint8_t getCPrev() const { return pC; }
    uint8_t getDPrev() const { return pD; }
    uint8_t getEPrev() const { return pE; }
    uint8_t getHPrev() const { return pH; }
    uint8_t getLPrev() const { return pL; }
    uint8_t getIFPrev() const { return pIF; }
    uint8_t getIEPrev() const { return pIE; }
    uint16_t getAFPrev() const { return pAF; }
    uint16_t getBCPrev() const { return pBC; }
    uint16_t getDEPrev() const { return pDE; }
    uint16_t getHLPrev() const { return pHL; }
    uint16_t getSPPrev() const { return pSP; }
    uint16_t getPCPrev() const { return pPC; }
    uint16_t getCurrInstruction() const { return currInstruction; }

    void reset() {
        A = F = B = C = D = E = H = L = 0;
        pA = pF = pB = pC = pD = pE = pH = pL = pIE = pIF = 0;
        pPC = pSP = pBC = pDE = pHL = 0;
        PC = SP = 0;
        IME = false;
        stopped = false;
        cycles = 0;

        timers->reset();
    }

    void printRegisters() {
        std::cout << "A = " << A << "B = " << B << "C = " << C << "D = " << D << "E = " << E
           << "F = " << F << "H = " << H << "L = " << L  << "BC = " << BC 
            << "DE = " << DE << "AF = " << AF << "HL = " << HL << "SP = " << SP
           << "PC = " << PC;
     }

    void runUntilRomStart() {
        while (PC.data != 0x100) {
            tick();
        }

        for (size_t i = 0; i < 0x100; ++i) {
            memory->getMemory()[i] = memory->getBootAreaRemap()[i];
        }
    }

    void runUntilCompletion() {
        while (PC.data != pPC) {
            tick();
        }
    }

    void runOneFrame() {
        while (!frameDone) {
            tick();
        }
        frameDone = false;
    }
};
