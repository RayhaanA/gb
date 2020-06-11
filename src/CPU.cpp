#include "CPU.hpp"

void CPU::tick() {
    uint16_t op = memory.read(PC++.data, cycles);
    if (op == 0xCB) {
        op = memory.read(PC++.data, cycles);
        callInstructionCB(op);
    }
    else {
        callInstruction(op);
    }
}

void CPU::callInstruction(uint16_t instruction) {
    switch (instruction) {
    case 0x00: NOP(); break;
    case 0x01: LD(BC, readU16()); break;
    case 0x02: LD(BC, A); break;
    case 0x03: INC(BC); break;
    case 0x04: INC(B); break;
    case 0x05: DEC(B); break;
    case 0x06: LD(B, readU8()); break;
    case 0x07: RLCA(); break;
    case 0x08: LD(readU16(), SP); break;
    case 0x09: ADD(HL, BC); break;
    case 0x0A: LD(A, BC); break;
    case 0x0B: DEC(BC); break;
    case 0x0C: INC(C); break;
    case 0x0D: DEC(C); break;
    case 0x0E: LD(C, readU8()); break;
    case 0x0F: RRCA(); break;
    case 0x10: STOP(); break;
    case 0x11: LD(DE, readU16()); break;
    case 0x12: LD(DE.getData(), A); break;
    case 0x13: INC(DE); break;
    case 0x14: INC(D); break;
    case 0x15: DEC(D); break;
    case 0x16: LD(D, readU8()); break;
    case 0x17: RLA(); break;
    case 0x18: JR(readS8()); break;
    case 0x19: ADD(HL, DE); break;
    case 0x1A: LD(A, DE.getData()); break;
    case 0x1B: DEC(DE); break;
    case 0x1C: INC(E); break;
    case 0x1D: DEC(E); break;
    case 0x1E: LD(E, readU8()); break;
    case 0x1F: RRA(); break;
    case 0x20: JR(!checkFlags(Z_FLAG), readS8()); break;
    case 0x21: LD(HL, readU16()); break;
    case 0x22: LDI(A, false); break;
    case 0x23: INC(HL); break;
    case 0x24: INC(H); break;
    case 0x25: DEC(H); break;
    case 0x26: LD(H, readU8()); break;
    case 0x27: DAA(); break;
    case 0x28: JR(checkFlags(Z_FLAG), readS8()); break;
    case 0x29: ADD(HL, HL); break;
    case 0x2A: LDI(A, true); break;
    case 0x2B: DEC(HL); break;
    case 0x2C: INC(L); break;
    case 0x2D: DEC(L); break;
    case 0x2E: LD(L, readU8()); break;
    case 0x2F: CPL(); break;
    case 0x30: JR(!checkFlags(C_FLAG), readS8()); break;
    case 0x31: LD(SP, readU16()); break;
    case 0x32: LDD(A, false); break;
    case 0x33: INC(SP); break;
    case 0x34: INC(HL.getData()); break;
    case 0x35: DEC(HL.getData()); break;
    case 0x36: LD(HL.getData(), readU8());
    case 0x37: SCF(); break;
    case 0x38: JR(checkFlags(C_FLAG), readS8()); break;
    case 0x39: ADD(HL, SP); break;
    case 0x3A: LDD(A, true); break;
    case 0x3B: DEC(SP); break;
    case 0x3C: INC(A); break;
    case 0x3D: DEC(A); break;
    case 0x3E: LD(A, readU8()); break;
    case 0x3F: CCF(); break;
    case 0x40: LD(B, B); break;
    case 0x41: LD(B, C); break;
    case 0x42: LD(B, D); break;
    case 0x43: LD(B, E); break;
    case 0x44: LD(B, H); break;
    case 0x45: LD(B, L); break;
    case 0x46: LD(B, HL.getData()); break;
    case 0x47: LD(B, A); break;
    case 0x48: LD(C, B); break;
    case 0x49: LD(C, C); break;
    case 0x4A: LD(C, D); break;
    case 0x4B: LD(C, E); break;
    case 0x4C: LD(C, H); break;
    case 0x4D: LD(C, L); break;
    case 0x4E: LD(C, HL.getData()); break;
    case 0x4F: LD(C, A); break;
    case 0x50: LD(D, B); break;
    case 0x51: LD(D, C); break;
    case 0x52: LD(D, D); break;
    case 0x53: LD(D, E); break;
    case 0x54: LD(D, H); break;
    case 0x55: LD(D, L); break;
    case 0x56: LD(D, HL.getData()); break;
    case 0x57: LD(D, A); break;
    case 0x58: LD(E, B); break;
    case 0x59: LD(E, C); break;
    case 0x5A: LD(E, D); break;
    case 0x5B: LD(E, E); break;
    case 0x5C: LD(E, H); break;
    case 0x5D: LD(E, L); break;
    case 0x5E: LD(E, HL.getData()); break;
    case 0x5F: LD(E, A); break;
    case 0x60: LD(H, B); break;
    case 0x61: LD(H, C); break;
    case 0x62: LD(H, D); break;
    case 0x63: LD(H, E); break;
    case 0x64: LD(H, H); break;
    case 0x65: LD(H, L); break;
    case 0x66: LD(H, HL.getData()); break;
    case 0x67: LD(H, A); break;
    case 0x68: LD(L, B); break;
    case 0x69: LD(L, C); break;
    case 0x6A: LD(L, D); break;
    case 0x6B: LD(L, E); break;
    case 0x6C: LD(L, H); break;
    case 0x6D: LD(L, L); break;
    case 0x6E: LD(L, HL.getData()); break;
    case 0x6F: LD(L, A); break;
    case 0x70: LD(HL.getData(), B); break;
    case 0x71: LD(HL.getData(), C); break;
    case 0x72: LD(HL.getData(), D); break;
    case 0x73: LD(HL.getData(), E); break;
    case 0x74: LD(HL.getData(), H); break;
    case 0x75: LD(HL.getData(), L); break;
    case 0x76: HALT(); break;
    case 0x77: LD(HL.getData(), A); break;
    case 0x78: LD(A, B); break;
    case 0x79: LD(A, C); break;
    case 0x7A: LD(A, D); break;
    case 0x7B: LD(A, E); break;
    case 0x7C: LD(A, H); break;
    case 0x7D: LD(A, L); break;
    case 0x7E: LD(A, HL.getData()); break;
    case 0x7F: LD(A, A); break;
    case 0x80: ADD(B); break;
    case 0x81: ADD(C); break;
    case 0x82: ADD(D); break;
    case 0x83: ADD(E); break;
    case 0x84: ADD(H); break;
    case 0x85: ADD(L); break;
    case 0x86: ADD(HL.getData()); break;
    case 0x87: ADD(A); break;
    case 0x88: ADC(B); break;
    case 0x89: ADC(C); break;
    case 0x8A: ADC(D); break;
    case 0x8B: ADC(E); break;
    case 0x8C: ADC(H); break;
    case 0x8D: ADC(L); break;
    case 0x8E: ADC(HL.getData()); break;
    case 0x8F: ADC(A); break;
    case 0x90: SUB(B); break;
    case 0x91: SUB(C); break;
    case 0x92: SUB(D); break;
    case 0x93: SUB(E); break;
    case 0x94: SUB(H); break;
    case 0x95: SUB(L); break;
    case 0x96: SUB(HL.getData()); break;
    case 0x97: SUB(A); break;
    case 0x98: SBC(B); break;
    case 0x99: SBC(C); break;
    case 0x9A: SBC(D); break;
    case 0x9B: SBC(E); break;
    case 0x9C: SBC(H); break;
    case 0x9D: SBC(L); break;
    case 0x9E: SBC(HL.getData()); break;
    case 0x9F: SBC(A); break;
    case 0xA0: AND(B); break;
    case 0xA1: AND(C); break;
    case 0xA2: AND(D); break;
    case 0xA3: AND(E); break;
    case 0xA4: AND(H); break;
    case 0xA5: AND(L); break;
    case 0xA6: AND(HL.getData()); break;
    case 0xA7: AND(A); break;
    case 0xA8: XOR(B); break;
    case 0xA9: XOR(C); break;
    case 0xAA: XOR(D); break;
    case 0xAB: XOR(E); break;
    case 0xAC: XOR(H); break;
    case 0xAD: XOR(L); break;
    case 0xAE: XOR(HL.getData()); break;
    case 0xAF: XOR(A); break;
    case 0xB0: OR(B); break;
    case 0xB1: OR(C); break;
    case 0xB2: OR(D); break;
    case 0xB3: OR(E); break;
    case 0xB4: OR(H); break;
    case 0xB5: OR(L); break;
    case 0xB6: OR(HL.getData()); break;
    case 0xB7: OR(A); break;
    case 0xB8: CP(B); break;
    case 0xB9: CP(C); break;
    case 0xBA: CP(D); break;
    case 0xBB: CP(E); break;
    case 0xBC: CP(H); break;
    case 0xBD: CP(L); break;
    case 0xBE: CP(HL.getData()); break;
    case 0xBF: CP(A); break;
    case 0xC0: RET(!checkFlags(Z_FLAG)); break;
    case 0xC1: POP(BC); break;
    case 0xC2: JP(!checkFlags(Z_FLAG), readU16()); break;
    case 0xC3: JP(readU16()); break;
    case 0xC4: CALL(!checkFlags(Z_FLAG), readU16()); break;
    case 0xC5: PUSH(BC); break;
    case 0xC6: ADD(readU8()); break;
    case 0xC7: RST(0x00); break;
    case 0xC8: RET(checkFlags(Z_FLAG)); break;
    case 0xC9: RET(); break;
    case 0xCA: JP(checkFlags(Z_FLAG), readU16()); break;
    case 0xCC: CALL(checkFlags(Z_FLAG), readU16()); break;
    case 0xCD: CALL(readU16()); break;
    case 0xCE: ADC(readU8()); break;
    case 0xCF: RST(0x08); break;
    case 0xD0: RET(!checkFlags(C_FLAG)); break;
    case 0xD1: POP(DE); break;
    case 0xD2: JP(!checkFlags(C_FLAG), readU16()); break;
    case 0xD4: CALL(!checkFlags(C_FLAG), readU16()); break;
    case 0xD5: PUSH(DE); break;
    case 0xD6: SUB(readU8()); break;
    case 0xD7: RST(0x10); break;
    case 0xD8: RET(checkFlags(C_FLAG)); break;
    case 0xD9: RETI(); break;
    case 0xDA: JP(checkFlags(C_FLAG), readU16()); break;
    case 0xDC: CALL(checkFlags(C_FLAG)); break;
    case 0xDE: SBC(readU8()); break;
    case 0xDF: RST(0x18); break;
    case 0xE0: LD(static_cast<uint16_t>(0xFF00u + readU8()), A); break;
    case 0xE1: POP(HL); break;
    case 0xE2: LD(static_cast<uint16_t>(0xFF00u + C.data), A); break;
    case 0xE5: PUSH(HL); break;
    case 0xE6: AND(readU8()); break;
    case 0xE7: RST(0x20); break;
    case 0xE8: ADD(SP, readS8()); break;
    case 0xE9: JP(HL.getData()); break;
    case 0xEA: LD(readU16(), A); break;
    case 0xEE: XOR(readU8()); break;
    case 0xEF: RST(0x28); break;
    case 0xF0: LD(A, static_cast<uint16_t>((0xFF00u + readU8()))); break;
    case 0xF1: POP(AF); break;
    case 0xF2: LD(A, static_cast<uint16_t>(0xFF00u + C.data)); break;
    case 0xF3: DI(); break;
    case 0xF5: PUSH(AF); break;
    case 0xF6: OR(readU8()); break;
    case 0xF7: RST(0x30); break;
    case 0xF8: LD(HL, SP.data + readS8()); break;
    case 0xF9: LD(SP, HL); break;
    case 0xFA: LD(A, readU16()); break;
    case 0xFB: EI(); break;
    case 0xFE: CP(readU8()); break;
    case 0xFF: RST(0x38); break;

    default:
        std::cout << "Unhandled instruction: " << std::hex << instruction
            << std::dec << "\n";
        break;
    }
}


void CPU::callInstructionCB(uint16_t instruction) {
    switch (instruction) {
    case 0x00: RLC(B); break;
    case 0x01: RLC(C); break;
    case 0x02: RLC(D); break;
    case 0x03: RLC(E); break;
    case 0x04: RLC(H); break;
    case 0x05: RLC(L); break;
    case 0x06: RLC(HL.getData()); break;
    case 0x07: RLC(A); break;
    case 0x08: RRC(B); break;
    case 0x09: RRC(C); break;
    case 0x0A: RRC(D); break;
    case 0x0B: RRC(E); break;
    case 0x0C: RRC(H); break;
    case 0x0D: RRC(L); break;
    case 0x0E: RRC(HL.getData()); break;
    case 0x0F: RRC(A); break;
    case 0x10: RL(B); break;
    case 0x11: RL(C); break;
    case 0x12: RL(D); break;
    case 0x13: RL(E); break;
    case 0x14: RL(H); break;
    case 0x15: RL(L); break;
    case 0x16: RL(HL.getData()); break;
    case 0x17: RL(A); break;
    case 0x18: RR(B); break;
    case 0x19: RR(C); break;
    case 0x1A: RR(D); break;
    case 0x1B: RR(E); break;
    case 0x1C: RR(H); break;
    case 0x1D: RR(L); break;
    case 0x1E: RR(HL.getData()); break;
    case 0x1F: RR(A); break;
    case 0x20: SLA(B); break;
    case 0x21: SLA(C); break;
    case 0x22: SLA(D); break;
    case 0x23: SLA(E); break;
    case 0x24: SLA(H); break;
    case 0x25: SLA(L); break;
    case 0x26: SLA(HL.getData()); break;
    case 0x27: SLA(A); break;
    case 0x28: SRA(B); break;
    case 0x29: SRA(C); break;
    case 0x2A: SRA(D); break;
    case 0x2B: SRA(E); break;
    case 0x2C: SRA(H); break;
    case 0x2D: SRA(L); break;
    case 0x2E: SRA(HL.getData()); break;
    case 0x2F: SRA(A); break;
    case 0x30: SWAP(B); break;
    case 0x31: SWAP(C); break;
    case 0x32: SWAP(D); break;
    case 0x33: SWAP(E); break;
    case 0x34: SWAP(H); break;
    case 0x35: SWAP(L); break;
    case 0x36: SWAP(HL.getData()); break;
    case 0x37: SWAP(A); break;
    case 0x38: SRL(B); break;
    case 0x39: SRL(C); break;
    case 0x3A: SRL(D); break;
    case 0x3B: SRL(E); break;
    case 0x3C: SRL(H); break;
    case 0x3D: SRL(L); break;
    case 0x3E: SRL(HL.getData()); break;
    case 0x3F: SRL(A); break;
    case 0x40: BIT(0, B); break;
    case 0x41: BIT(0, C); break;
    case 0x42: BIT(0, D); break;
    case 0x43: BIT(0, E); break;
    case 0x44: BIT(0, H); break;
    case 0x45: BIT(0, L); break;
    case 0x46: BIT(0, HL.getData()); break;
    case 0x47: BIT(0, A); break;
    case 0x48: BIT(1, B); break;
    case 0x49: BIT(1, C); break;
    case 0x4A: BIT(1, D); break;
    case 0x4B: BIT(1, E); break;
    case 0x4C: BIT(1, H); break;
    case 0x4D: BIT(1, L); break;
    case 0x4E: BIT(1, HL.getData()); break;
    case 0x4F: BIT(1, A); break;
    case 0x50: BIT(2, B); break;
    case 0x51: BIT(2, C); break;
    case 0x52: BIT(2, D); break;
    case 0x53: BIT(2, E); break;
    case 0x54: BIT(2, H); break;
    case 0x55: BIT(2, L); break;
    case 0x56: BIT(2, HL.getData()); break;
    case 0x57: BIT(2, A); break;
    case 0x58: BIT(3, B); break;
    case 0x59: BIT(3, C); break;
    case 0x5A: BIT(3, D); break;
    case 0x5B: BIT(3, E); break;
    case 0x5C: BIT(3, H); break;
    case 0x5D: BIT(3, L); break;
    case 0x5E: BIT(3, HL.getData()); break;
    case 0x5F: BIT(3, A); break;
    case 0x60: BIT(4, B); break;
    case 0x61: BIT(4, C); break;
    case 0x62: BIT(4, D); break;
    case 0x63: BIT(4, E); break;
    case 0x64: BIT(4, H); break;
    case 0x65: BIT(4, L); break;
    case 0x66: BIT(4, HL.getData()); break;
    case 0x67: BIT(4, A); break;
    case 0x68: BIT(5, B); break;
    case 0x69: BIT(5, C); break;
    case 0x6A: BIT(5, D); break;
    case 0x6B: BIT(5, E); break;
    case 0x6C: BIT(5, H); break;
    case 0x6D: BIT(5, L); break;
    case 0x6E: BIT(5, HL.getData()); break;
    case 0x6F: BIT(5, A); break;
    case 0x70: BIT(6, B); break;
    case 0x71: BIT(6, C); break;
    case 0x72: BIT(6, D); break;
    case 0x73: BIT(6, E); break;
    case 0x74: BIT(6, H); break;
    case 0x75: BIT(6, L); break;
    case 0x76: BIT(6, HL.getData()); break;
    case 0x77: BIT(6, A); break;
    case 0x78: BIT(7, B); break;
    case 0x79: BIT(7, C); break;
    case 0x7A: BIT(7, D); break;
    case 0x7B: BIT(7, E); break;
    case 0x7C: BIT(7, H); break;
    case 0x7D: BIT(7, L); break;
    case 0x7E: BIT(7, HL.getData()); break;
    case 0x7F: BIT(7, A); break;
    case 0x80: RES(0, B); break;
    case 0x81: RES(0, C); break;
    case 0x82: RES(0, D); break;
    case 0x83: RES(0, E); break;
    case 0x84: RES(0, H); break;
    case 0x85: RES(0, L); break;
    case 0x86: RES(0, HL.getData()); break;
    case 0x87: RES(0, A); break;
    case 0x88: RES(1, B); break;
    case 0x89: RES(1, C); break;
    case 0x8A: RES(1, D); break;
    case 0x8B: RES(1, E); break;
    case 0x8C: RES(1, H); break;
    case 0x8D: RES(1, L); break;
    case 0x8E: RES(1, HL.getData()); break;
    case 0x8F: RES(1, A); break;
    case 0x90: RES(2, B); break;
    case 0x91: RES(2, C); break;
    case 0x92: RES(2, D); break;
    case 0x93: RES(2, E); break;
    case 0x94: RES(2, H); break;
    case 0x95: RES(2, L); break;
    case 0x96: RES(2, HL.getData()); break;
    case 0x97: RES(2, A); break;
    case 0x98: RES(3, B); break;
    case 0x99: RES(3, C); break;
    case 0x9A: RES(3, D); break;
    case 0x9B: RES(3, E); break;
    case 0x9C: RES(3, H); break;
    case 0x9D: RES(3, L); break;
    case 0x9E: RES(3, HL.getData()); break;
    case 0x9F: RES(3, A); break;
    case 0xA0: RES(4, B); break;
    case 0xA1: RES(4, C); break;
    case 0xA2: RES(4, D); break;
    case 0xA3: RES(4, E); break;
    case 0xA4: RES(4, H); break;
    case 0xA5: RES(4, L); break;
    case 0xA6: RES(4, HL.getData()); break;
    case 0xA7: RES(4, A); break;
    case 0xA8: RES(5, B); break;
    case 0xA9: RES(5, C); break;
    case 0xAA: RES(5, D); break;
    case 0xAB: RES(5, E); break;
    case 0xAC: RES(5, H); break;
    case 0xAD: RES(5, L); break;
    case 0xAE: RES(5, HL.getData()); break;
    case 0xAF: RES(5, A); break;
    case 0xB0: RES(6, B); break;
    case 0xB1: RES(6, C); break;
    case 0xB2: RES(6, D); break;
    case 0xB3: RES(6, E); break;
    case 0xB4: RES(6, H); break;
    case 0xB5: RES(6, L); break;
    case 0xB6: RES(6, HL.getData()); break;
    case 0xB7: RES(6, A); break;
    case 0xB8: RES(7, B); break;
    case 0xB9: RES(7, C); break;
    case 0xBA: RES(7, D); break;
    case 0xBB: RES(7, E); break;
    case 0xBC: RES(7, H); break;
    case 0xBD: RES(7, L); break;
    case 0xBE: RES(7, HL.getData()); break;
    case 0xBF: RES(7, A); break;
    case 0xC0: SET(0, B); break;
    case 0xC1: SET(0, C); break;
    case 0xC2: SET(0, D); break;
    case 0xC3: SET(0, E); break;
    case 0xC4: SET(0, H); break;
    case 0xC5: SET(0, L); break;
    case 0xC6: SET(0, HL.getData()); break;
    case 0xC7: SET(0, A); break;
    case 0xC8: SET(1, B); break;
    case 0xC9: SET(1, C); break;
    case 0xCA: SET(1, D); break;
    case 0xCB: SET(1, E); break;
    case 0xCC: SET(1, H); break;
    case 0xCD: SET(1, L); break;
    case 0xCE: SET(1, HL.getData()); break;
    case 0xCF: SET(1, A); break;
    case 0xD0: SET(2, B); break;
    case 0xD1: SET(2, C); break;
    case 0xD2: SET(2, D); break;
    case 0xD3: SET(2, E); break;
    case 0xD4: SET(2, H); break;
    case 0xD5: SET(2, L); break;
    case 0xD6: SET(2, HL.getData()); break;
    case 0xD7: SET(2, A); break;
    case 0xD8: SET(3, B); break;
    case 0xD9: SET(3, C); break;
    case 0xDA: SET(3, D); break;
    case 0xDB: SET(3, E); break;
    case 0xDC: SET(3, H); break;
    case 0xDD: SET(3, L); break;
    case 0xDE: SET(3, HL.getData()); break;
    case 0xDF: SET(3, A); break;
    case 0xE0: SET(4, B); break;
    case 0xE1: SET(4, C); break;
    case 0xE2: SET(4, D); break;
    case 0xE3: SET(4, E); break;
    case 0xE4: SET(4, H); break;
    case 0xE5: SET(4, L); break;
    case 0xE6: SET(4, HL.getData()); break;
    case 0xE7: SET(4, A); break;
    case 0xE8: SET(5, B); break;
    case 0xE9: SET(5, C); break;
    case 0xEA: SET(5, D); break;
    case 0xEB: SET(5, E); break;
    case 0xEC: SET(5, H); break;
    case 0xED: SET(5, L); break;
    case 0xEE: SET(5, HL.getData()); break;
    case 0xEF: SET(5, A); break;
    case 0xF0: SET(6, B); break;
    case 0xF1: SET(6, C); break;
    case 0xF2: SET(6, D); break;
    case 0xF3: SET(6, E); break;
    case 0xF4: SET(6, H); break;
    case 0xF5: SET(6, L); break;
    case 0xF6: SET(6, HL.getData()); break;
    case 0xF7: SET(6, A); break;
    case 0xF8: SET(7, B); break;
    case 0xF9: SET(7, C); break;
    case 0xFA: SET(7, D); break;
    case 0xFB: SET(7, E); break;
    case 0xFC: SET(7, H); break;
    case 0xFD: SET(7, L); break;
    case 0xFE: SET(7, HL.getData()); break;
    case 0xFF: SET(7, A); break;

    default:
        std::cout << "Unhandled CB instruction: CB" << std::hex << instruction
            << std::dec << "\n";
        break;
    }
}

//  Utilities
uint16_t CPU::makeU16(uint8_t lsb, uint8_t msb) {
    uint16_t temp = lsb;
    temp |= msb << 8;

    return temp;
}

uint8_t CPU::readU8() {
    uint8_t data = memory.read(PC++.data, cycles);
    return data;
}

uint16_t CPU::readU16() {
    uint8_t low = readU8();
    uint8_t high = readU8();
    return makeU16(low, high);
}

int8_t CPU::readS8()
{
    int8_t data = static_cast<int8_t>(memory.read(PC++.data, cycles));
    return data;
}

void CPU::setFlags(uint8_t value) {
    F.data |= value;
}

void CPU::unsetFlags(uint8_t value) {
    F.data &= ~value;
}

bool CPU::checkFlags(uint8_t value) {
    return F.data & value;
}



/*
*  OP CODE IMPLEMENTATIONS
*/


void CPU::addByteValueToA(uint8_t data) {
    unsetFlags(N_FLAG);

    if (((A.data & 0xF) + (data & 0xF)) & 0x10) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (A.data + data > std::numeric_limits<uint8_t>::max()) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    A.data += data;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::addTwoByteValueToRegisterPair(RegisterPair& dest, uint16_t data) {
    unsetFlags(N_FLAG);

    if (((dest.getData() & 0xFF) + (data & 0xFF)) & 0x0100) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (dest.getData() + data > std::numeric_limits<uint16_t>::max()) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    dest = dest.getData() + data;
}

void CPU::subByteValueFromA(uint8_t data) {
    setFlags(N_FLAG);

    if ((static_cast<int16_t>(A.data & 0xF) - static_cast<int16_t>(data & 0xF)) < 0) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (A.data < data) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    A.data -= data;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::andByteValueAgainstA(uint8_t data)
{
    unsetFlags(N_FLAG | C_FLAG);
    setFlags(H_FLAG);

    A.data &= data;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::orByteValueAgainstA(uint8_t data)
{
    unsetFlags(H_FLAG | N_FLAG | C_FLAG);

    A.data |= data;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::xorByteValueAgainstA(uint8_t data)
{
    unsetFlags(H_FLAG | N_FLAG | C_FLAG);

    A.data ^= data;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::cpByteValueAgainstA(uint8_t data)
{
    setFlags(N_FLAG);

    if (A.data == data) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    if ((A.data & 0xF) - (data & 0xF) < 0) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (A.data < data) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }
}

void CPU::adcByteValueAgainstA(uint8_t data) {
    unsetFlags(N_FLAG);

    bool carry = checkFlags(C_FLAG) ? 1 : 0;

    if (((A.data & 0xF) + (data & 0xF) + carry) & 0x10) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (A.data + data + carry > std::numeric_limits<uint8_t>::max()) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    A.data += data + carry;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::sbcByteValueAgainstA(uint8_t data) {
    setFlags(N_FLAG);

    bool carry = checkFlags(C_FLAG) ? 1 : 0;

    if ((static_cast<int16_t>(A.data & 0xF) - static_cast<int16_t>(data & 0xF) - carry) < 0) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (A.data < data + carry) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    A.data -= data - carry;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::NOP() {
    return;
}

void CPU::STOP() {
    stopped = true;
}

void CPU::HALT() {
    return;
}

void CPU::JP(uint16_t address) {
    PC = address;
}

void CPU::JP(bool condition, uint16_t address) {
    if (condition) {
        PC = address;
    }
}

void CPU::JR(int8_t offset) {
    PC = PC.data + offset;
    ++cycles;
}

void CPU::JR(bool condition, int8_t offset) {
    if (condition) {
        PC = PC.data + offset;
        ++cycles;
    }
}

void CPU::CALL(uint16_t address) {
    memory.write(PC.getMSB(), --SP.data, cycles);
    memory.write(PC.getLSB(), --SP.data, cycles);

    PC = address;
}

void CPU::CALL(bool condition, uint16_t address) {
    if (condition) {
        memory.write(PC.getMSB(), --SP.data, cycles);
        memory.write(PC.getLSB(), --SP.data, cycles);

        PC = address;
    }
}

void CPU::RET() {
    uint8_t low = memory.read(SP++.data, cycles);
    uint8_t high = memory.read(SP++.data, cycles);

    PC = makeU16(low, high);
}

void CPU::RET(bool condition) {
    if (condition) {
        uint8_t low = memory.read(SP++.data, cycles);
        uint8_t high = memory.read(SP++.data, cycles);

        PC = makeU16(low, high);
    }
}

void CPU::RETI() {
    uint8_t low = memory.read(SP++.getLSB(), cycles);
    uint8_t high = memory.read(SP++.getMSB(), cycles);

    PC = makeU16(low, high);

    IME = true;
}

void CPU::RST(uint8_t lsb) {
    memory.write(PC.getMSB(), --SP.data, cycles);
    memory.write(PC.getLSB(), --SP.data, cycles);

    PC = makeU16(lsb, 0x00);
}

void CPU::DI() {
    IME = false;
}

void CPU::EI() {
    IME = true;
}

void CPU::CCF() {
    unsetFlags(N_FLAG | H_FLAG);
    checkFlags(C_FLAG) ? unsetFlags(C_FLAG) : setFlags(C_FLAG);
}

void CPU::SCF() {
    setFlags(C_FLAG);
    unsetFlags(N_FLAG | H_FLAG);
}

void CPU::DAA() {
    uint16_t a = A.data;

    if (!(checkFlags(N_FLAG))) {
        if (checkFlags(H_FLAG) || ((a & 0xF) > 9))
            a += 0x06;

        if (checkFlags(C_FLAG) || (a > 0x9F))
            a += 0x60;
    }
    else {
        if (checkFlags(H_FLAG)) {
            a = (a - 6) & 0xFF;
        }

        if (checkFlags(C_FLAG)) {
            a -= 0x60;
        }
    }

    setFlags(H_FLAG | Z_FLAG);

    if (a > std::numeric_limits<uint8_t>::max()) {
        setFlags(C_FLAG);
    }

    a &= 0xFF;

    A.data = a & 0xFF;

    if (A.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::CPL() {
    A.data = ~A.data;
    setFlags(N_FLAG | H_FLAG);
}

void CPU::LD(Register& dest, Register src) {
    dest = src;
}

void CPU::LD(Register& dest, uint16_t address) {
    dest.data = memory.read(address, cycles);
}

void CPU::LD(RegisterPair address, Register src) {
    memory.write(src.data, address.getData(), cycles);
}

void CPU::LD(Register& dest, uint8_t value) {
    dest = value;
}

void CPU::LD(RegisterPair& dest, uint16_t value) {
    dest = value;
}

void CPU::LD(Register& dest, RegisterPair address) {
    dest = memory.read(address.getData(), cycles);
}

void CPU::LD(SpecialRegister& dest, RegisterPair src) {
    dest.data = src.getData();
}

void CPU::LD(SpecialRegister& dest, uint16_t value) {
    dest.data = value;
}

void CPU::LD(uint16_t address, SpecialRegister src) {
    memory.write(src.getLSB(), address, cycles);
    memory.write(src.getMSB(), address, cycles);
}

void CPU::LD(uint16_t address, Register src) {
    memory.write(src.data, address, cycles);
}

void CPU::LD(RegisterPair& dest, SpecialRegister src, int8_t offset) {
    unsetFlags(Z_FLAG | N_FLAG | H_FLAG | C_FLAG);

    if (offset >= 0) {
        if (((src.data & 0xFF) + offset) > 0xFF) {
            setFlags(C_FLAG);
        }
        if (((src.data & 0xF) + (offset & 0xF)) > 0xF) {
            setFlags(H_FLAG);
        }
    }
    else {
        if (((src.data + offset) & 0xFF) <= (src.data & 0xFF)) {
            setFlags(C_FLAG);
        }
        if (((src.data + offset) & 0xF) <= (src.data & 0xF)) {
            setFlags(H_FLAG);
        }
    }

    dest = src.data + offset;
}

void CPU::LDI(Register src, bool setA) {
    setA ? A = memory.read(HL.getData(), cycles) : memory.write(src.data, HL.getData(), cycles);
    ++HL;
}

void CPU::LDD(Register src, bool setA) {
    setA ? A = memory.read(HL.getData(), cycles) : memory.write(src.data, HL.getData(), cycles);
    --HL;
}
void CPU::INC(Register& r) {
    if (((r.data & 0xF) + 1) & 0x10) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    ++r;

    unsetFlags(N_FLAG);

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::INC(RegisterPair& r) {
    ++r;
    ++cycles;
}

void CPU::INC(SpecialRegister& r) {
    ++r;
    ++cycles;
}

void CPU::INC(uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    memory.write(++data, address, cycles);
}

void CPU::DEC(Register& r) {
    if ((static_cast<int8_t>(r.data) & 0xF - 1) < 0) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    --r;

    setFlags(N_FLAG);

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::DEC(RegisterPair& r) {
    --r;
    --cycles;
}

void CPU::DEC(SpecialRegister& r) {
    --r;
}

void CPU::DEC(uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    memory.write(--data, address, cycles);
}

void CPU::ADD(Register src) {

    addByteValueToA(src.data);
}

void CPU::ADD(uint8_t data)
{
    addByteValueToA(data);
}

void CPU::ADD(uint16_t address) {
    addByteValueToA(memory.read(address, cycles));
}

void CPU::ADD(RegisterPair& dest, RegisterPair src) {
    addTwoByteValueToRegisterPair(dest, src.getData());
    ++cycles;
}

void CPU::ADD(SpecialRegister& dest, int8_t data)
{
    unsetFlags(Z_FLAG | N_FLAG | H_FLAG | C_FLAG);

    if (data >= 0) {
        if (((dest.data & 0xFF) + data) > 0xFF) {
            setFlags(C_FLAG);
        }
        if (((dest.data & 0xF) + (data & 0xF)) > 0xF) {
            setFlags(H_FLAG);
        }
    }
    else {
        if (((dest.data + data) & 0xFF) <= (dest.data & 0xFF)) {
            setFlags(C_FLAG);
        }
        if (((dest.data + data) & 0xF) <= (dest.data & 0xF)) {
            setFlags(H_FLAG);
        }
    }

    dest.data = dest.data + data;
}

void CPU::ADD(RegisterPair& dest, SpecialRegister src) {
    addTwoByteValueToRegisterPair(dest, src.data);
}

void CPU::SUB(Register src) {
    subByteValueFromA(src.data);
}

void CPU::SUB(uint16_t address) {
    subByteValueFromA(memory.read(address, cycles));
}

void CPU::SUB(uint8_t data) {
    subByteValueFromA(data);
}

void CPU::AND(Register src) {
    andByteValueAgainstA(src.data);
}

void CPU::AND(uint16_t address) {
    andByteValueAgainstA(memory.read(address, cycles));
}

void CPU::AND(uint8_t data) {
    andByteValueAgainstA(data);
}

void CPU::OR(Register src) {
    orByteValueAgainstA(src.data);
}

void CPU::OR(uint16_t address) {
    orByteValueAgainstA(memory.read(address, cycles));
}

void CPU::OR(uint8_t data) {
    orByteValueAgainstA(data);
}

void CPU::XOR(Register src) {
    xorByteValueAgainstA(src.data);
}

void CPU::XOR(uint16_t address) {
    xorByteValueAgainstA(memory.read(address, cycles));
}

void CPU::XOR(uint8_t data) {
    xorByteValueAgainstA(data);
}

void CPU::CP(Register src) {
    cpByteValueAgainstA(src.data);
}

void CPU::CP(uint16_t address) {
    cpByteValueAgainstA(memory.read(address, cycles));

}

void CPU::CP(uint8_t data) {
    cpByteValueAgainstA(data);
}

void CPU::ADC(Register src) {
    adcByteValueAgainstA(src.data);
}

void CPU::ADC(uint16_t address) {
    adcByteValueAgainstA(memory.read(address, cycles));
}

void CPU::ADC(uint8_t data) {
    adcByteValueAgainstA(data);
}

void CPU::SBC(Register src) {
    sbcByteValueAgainstA(src.data);
}

void CPU::SBC(uint16_t address) {
    sbcByteValueAgainstA(memory.read(address, cycles));
}

void CPU::SBC(uint8_t data) {
    sbcByteValueAgainstA(data);
}

void CPU::POP(RegisterPair& dest) {
    dest.low->data = memory.read(SP++.data, cycles);
    dest.high->data = memory.read(SP++.data, cycles);

    if (dest.low.get() == &F) {
        F.data &= 0xF0;
    }
}

void CPU::PUSH(RegisterPair& dest) {
    memory.write(dest.getMSB(), --SP.data, cycles);
    memory.write(dest.getLSB(), --SP.data, cycles);
}

void CPU::RLA() {
    bool carry = A.data & 0x80;

    A.data <<= 1;

    A.data |= checkFlags(C_FLAG) ? 0x1 : 0x0;

    if (carry) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(Z_FLAG | H_FLAG | N_FLAG);
}

void CPU::RLCA() {
    uint8_t carry = A.data & 0x80;

    A.data <<= 1;

    A.data |= carry;

    if (carry) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(Z_FLAG | H_FLAG | N_FLAG);
}

void CPU::RRA() {
    bool carry = A.data & 0x1;

    A.data >>= 1;

    A.data |= checkFlags(C_FLAG) ? (0x1 << 7) : 0x0;

    if (carry) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(Z_FLAG | H_FLAG | N_FLAG);
}

void CPU::RRCA() {
    bool carry = A.data & 0x1;

    A.data >>= 1;

    A.data |= carry << 7;

    if (carry) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(Z_FLAG | H_FLAG | N_FLAG);
}

void CPU::RLC(Register& r) {
    bool msbSet = r.data & 0x80;
    r.data <<= 1;

    if (msbSet) {
        setFlags(C_FLAG);
        r.data |= 0x1;
    }
    else {
        unsetFlags(C_FLAG);
    }

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);
}

void CPU::RLC(uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    bool msbSet = data & 0x80;

    data <<= 1;

    if (msbSet) {
        setFlags(C_FLAG);
        data |= 0x1;
    }
    else {
        unsetFlags(C_FLAG);
    }

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    memory.write(data, address, cycles);
}

void CPU::RRC(Register& r) {
    bool lsbSet = r.data & 0x1;
    r.data >>= 1;

    if (lsbSet) {
        setFlags(C_FLAG);
        r.data |= 0x80;
    }
    else {
        unsetFlags(C_FLAG);
    }

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);
}

void CPU::RRC(uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    bool lsbSet = data & 0x1;

    data >>= 1;

    if (lsbSet) {
        setFlags(C_FLAG);
        data |= 0x80;
    }
    else {
        unsetFlags(C_FLAG);
    }

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    memory.write(data, address, cycles);
}

void CPU::RL(Register& r) {
    uint16_t shiftedData = r.data << 1;

    shiftedData |= checkFlags(C_FLAG) ? 0x1 : 0x0;

    if (shiftedData & 0x100) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    r.data = shiftedData & 0xFF;

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);
}

void CPU::RL(uint16_t address) {
    uint16_t shiftedData = memory.read(address, cycles) << 1;

    shiftedData |= checkFlags(C_FLAG) ? 0x1 : 0x0;

    if (shiftedData & 0x100) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    if ((shiftedData & 0xFF) == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    memory.write(shiftedData & 0xFF, address, cycles);
}

void CPU::RR(Register& r) {
    uint16_t data = r.data;

    uint8_t carry = checkFlags(C_FLAG);

    if (data & 0x1) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    data >>= 1;
    data |= carry << 0x7;

    r.data = data & 0xFF;

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);
}

void CPU::RR(uint16_t address) {
    uint8_t data = memory.read(address, cycles);

    bool carry = checkFlags(C_FLAG);

    if (data & 0x1) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    data >>= 1;
    data |= carry << 0x7;

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    memory.write(data, address, cycles);
}

void CPU::SLA(Register& r) {
    if (r.data & 0x80) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    r.data <<= 1;

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::SLA(uint16_t address) {
    uint8_t data = memory.read(address, cycles);

    if (data & 0x80) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    data <<= 1;

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    memory.write(data, address, cycles);
}
void CPU::SRA(Register& r) {
    if (r.data & 0x01) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    bool msbSet = r.data & 0x80;

    r.data >>= 1;
    r.data |= msbSet << 7;

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::SRA(uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    bool msbSet = data & 0x80;

    if (data & 0x01) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    data >>= 1;
    data |= msbSet << 7;

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    memory.write(data, address, cycles);
}

void CPU::SWAP(Register& r) {
    uint8_t low = r.data & 0xF;
    uint8_t high = (r.data & 0xF0) >> 4;

    r.data = high | (low << 4);

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    unsetFlags(C_FLAG | H_FLAG | N_FLAG);
}

void CPU::SWAP(uint16_t address) {
    uint8_t data = memory.read(address, cycles);

    uint8_t low = data & 0xF;
    uint8_t high = (data & 0xF0) >> 4;

    data = high | (low << 4);

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    memory.write(data, address, cycles);

    unsetFlags(C_FLAG | H_FLAG | N_FLAG);
}

void CPU::SRL(Register& r) {
    if (r.data & 0x01) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    r.data >>= 1;

    if (r.data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::SRL(uint16_t address) {
    uint8_t data = memory.read(address, cycles);

    if (data & 0x01) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    unsetFlags(H_FLAG | N_FLAG);

    data >>= 1;

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    memory.write(data, address, cycles);
}

void CPU::BIT(uint8_t position, Register src) {
    setFlags(H_FLAG);
    unsetFlags(N_FLAG);

    if (!(src.data & (1 << position))) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::BIT(uint8_t position, uint16_t address) {
    uint8_t data = memory.read(address, cycles);

    setFlags(H_FLAG);
    unsetFlags(N_FLAG);

    if (!(data & (1 << position))) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }
}

void CPU::RES(uint8_t position, Register src) {
    src.data &= ~(1 << position);
}

void CPU::RES(uint8_t position, uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    data &= ~(1 << position);
    memory.write(data, address, cycles);
}

void CPU::SET(uint8_t position, Register src) {
    src.data |= 1 << position;
}

void CPU::SET(uint8_t position, uint16_t address) {
    uint8_t data = memory.read(address, cycles);
    data |= 1 << position;
    memory.write(data, address, cycles);
}
