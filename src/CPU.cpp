#pragma once
#include "CPU.hpp"

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
    case 0x0C: INC(C); break;
    case 0x0E: LD(C, readU8()); break;
    case 0x11: LD(DE, readU16()); break;
    case 0x1A: LD(A, DE.getData()); break;
    case 0x20: JR(!checkFlags(Z_FLAG), readS8()); break;
    case 0x21: LD(HL, readU16()); break;
    case 0x22: LDI(A, false); break;
    case 0x23: INC(HL); break;
    case 0x2E: INC(DE); break;
    case 0x31: LD(SP, readU16()); break;
    case 0x32: LDD(A, false); break;
    case 0x3C: INC(A); break;
    case 0x3E: LD(A, readU8()); break;
    case 0x77: LD(HL.getData(), A); break;
    case 0x7B: LD(A, E); break;
    case 0xAF: XOR(A); break;
    case 0xC9: RET(); break;
    case 0xCD: CALL(readU16()); break;
    case 0xE0: LD(0xFF00 + readU8(), A); break;
    case 0xE2: LD(0xFF00 + C.data, A); break;
    case 0xEA: LD(makeU16(), A); break;
    case 0xFE: CP(readU8()); break;
    default:
        std::cout << "Unhandled instruction: " << std::hex << instruction
            << std::dec << "\n";
        break;
    }
}

void CPU::callInstructionCB(uint16_t instruction) {
    switch (instruction) {
    case 0x7C: BIT(7, H); break;
    default:
        std::cout << "Unhandled CB instruction: CB" << std::hex << instruction
            << std::dec << "\n";
        break;
    }
}

void CPU::tick()
{
    uint16_t op = memory.read(PC++.data);
    if (op == 0xCB) {
        op = memory.read(PC++.data);
        callInstructionCB(op);
    }
    else {
        callInstruction(op);
    }
}

//  Utilities
uint16_t CPU::makeU16(uint8_t lsb, uint8_t msb)
{
    uint16_t temp = lsb;
    temp |= msb << 8;

    return temp;
}

uint8_t CPU::readU8()
{
    uint8_t data = memory.read(PC++.data);
    return data;
}

uint16_t CPU::readU16() {
    uint8_t low = readU8();
    uint8_t high = readU8();
    return makeU16(low, high);
}

int8_t CPU::readS8()
{
    int8_t data = memory.read(PC++.data);
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
    PC = static_cast<int16_t>(PC.data) + offset;
}

void CPU::JR(bool condition, int8_t offset) {
    if (condition) {
        PC = static_cast<uint16_t>(static_cast<int32_t>(PC.data) + offset);
    }
}

void CPU::CALL(uint16_t address) {
    memory.write(PC.getMSB(), --SP.data);
    memory.write(PC.getLSB(), --SP.data);

    PC = address;
}

void CPU::CALL(bool condition, uint16_t address) {
    if (condition) {
        memory.write(PC.getMSB(), --SP.data);
        memory.write(PC.getLSB(), --SP.data);

        PC = address;
    }
}

void CPU::RET() {
    uint8_t low = memory.read(SP++.data);
    uint8_t high = memory.read(SP++.data);

    PC = makeU16(low, high);
}

void CPU::RET(bool condition) {
    if (condition) {
        uint8_t low = memory.read(SP++.data);
        uint8_t high = memory.read(SP++.data);

        PC = makeU16(low, high);
    }
}

void CPU::RETI() {
    uint8_t low = memory.read(SP++.getLSB());
    uint8_t high = memory.read(SP++.getMSB());

    PC = makeU16(low, high);

    IME = true;
}

void CPU::RST(uint8_t lsb) {
    memory.write(PC.getMSB(), --SP.data);
    memory.write(PC.getLSB(), --SP.data);

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

void CPU::LD(RegisterPair address, Register src) {
    memory.write(src.data, address.getData());
}

void CPU::LD(Register& dest, uint8_t value) {
    dest = value;
}

void CPU::LD(RegisterPair& dest, uint16_t value) {
    dest = value;
}

void CPU::LD(Register& dest, RegisterPair address) {
    dest = memory.read(address.getData());
}

void CPU::LD(SpecialRegister& dest, RegisterPair src) {
    dest.data = src.getData();
}

void CPU::LD(SpecialRegister& dest, uint16_t value) {
    dest.data = value;
}

void CPU::LD(uint16_t address, SpecialRegister src) {
    memory.write(src.getLSB(), address);
    memory.write(src.getMSB(), address);
}

void CPU::LD(uint16_t address, Register src)
{
    memory.write(src.data, address);
}

void CPU::LD(RegisterPair& dest, SpecialRegister src, int8_t offset) {
    unsetFlags(Z_FLAG | N_FLAG);
    int32_t srcData = static_cast<int32_t>(src.data);

    if (((srcData & 0xF) + (offset & 0xF)) & 0x10) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if ((srcData & 0xFF + offset) & 0xFF00) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    dest = static_cast<uint16_t>(srcData + offset);
}

void CPU::LDI(Register src, bool setA) {
    setA ? A = memory.read(HL.getData()) : memory.write(src.data, HL.getData());
    ++HL;
}

void CPU::LDD(Register src, bool setA) {
    setA ? A = memory.read(HL.getData()) : memory.write(src.data, HL.getData());
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
}

void CPU::INC(SpecialRegister& r) {
    ++r;
}

void CPU::INC(uint16_t address) {
    uint8_t data = memory.read(address);
    memory.write(++data, address);
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
}

void CPU::DEC(SpecialRegister& r) {
    --r;
}

void CPU::DEC(uint16_t address) {
    uint8_t data = memory.read(address);
    memory.write(--data, address);
}

void CPU::ADD(Register src) {

    addByteValueToA(src.data);
}

void CPU::ADD(uint8_t data)
{
    addByteValueToA(data);
}

void CPU::ADD(uint16_t address) {
    addByteValueToA(memory.read(address));
}

void CPU::ADD(RegisterPair& dest, RegisterPair src) {
    addTwoByteValueToRegisterPair(dest, src.getData());
}

void CPU::ADD(SpecialRegister& dest, int8_t data)
{
    unsetFlags(Z_FLAG | N_FLAG);
    int32_t srcVal = static_cast<int32_t>(dest.data);

    if (((srcVal & 0xF) + (data & 0xF)) & 0x10) {
        setFlags(H_FLAG);
    }
    else {
        unsetFlags(H_FLAG);
    }

    if (((srcVal & 0xFF) + data) & 0xFF00) {
        setFlags(C_FLAG);
    }
    else {
        unsetFlags(C_FLAG);
    }

    dest.data = static_cast<uint16_t>(srcVal + data);
}

void CPU::ADD(RegisterPair& dest, SpecialRegister src) {
    addTwoByteValueToRegisterPair(dest, src.data);
}

void CPU::SUB(Register src) {
    subByteValueFromA(src.data);
}

void CPU::SUB(uint16_t address) {
    subByteValueFromA(memory.read(address));
}

void CPU::SUB(uint8_t data) {
    subByteValueFromA(data);
}

void CPU::AND(Register src) {
    andByteValueAgainstA(src.data);
}

void CPU::AND(uint16_t address) {
    andByteValueAgainstA(memory.read(address));
}

void CPU::AND(uint8_t data) {
    andByteValueAgainstA(data);
}

void CPU::OR(Register src) {
    orByteValueAgainstA(src.data);
}

void CPU::OR(uint16_t address) {
    orByteValueAgainstA(memory.read(address));
}

void CPU::OR(uint8_t data) {
    orByteValueAgainstA(data);
}

void CPU::XOR(Register src) {
    xorByteValueAgainstA(src.data);
}

void CPU::XOR(uint16_t address) {
    xorByteValueAgainstA(memory.read(address));
}

void CPU::XOR(uint8_t data) {
    xorByteValueAgainstA(data);
}

void CPU::CP(Register src) {
    cpByteValueAgainstA(src.data);
}

void CPU::CP(uint16_t address) {
    cpByteValueAgainstA(memory.read(address));

}

void CPU::CP(uint8_t data) {
    cpByteValueAgainstA(data);
}

void CPU::ADC(Register src) {
    adcByteValueAgainstA(src.data);
}

void CPU::ADC(uint16_t address) {
    adcByteValueAgainstA(memory.read(address));
}

void CPU::ADC(uint8_t data) {
    adcByteValueAgainstA(data);
}

void CPU::SBC(Register src) {
    sbcByteValueAgainstA(src.data);
}

void CPU::SBC(uint16_t address) {
    sbcByteValueAgainstA(memory.read(address));
}

void CPU::SBC(uint8_t data) {
    sbcByteValueAgainstA(data);
}

void CPU::POP(RegisterPair& dest) {
    dest.low->data = memory.read(SP++.data);
    dest.high->data = memory.read(SP++.data);

    if (dest.low.get() == &F) {
        F.data &= 0xF0;
    }
}

void CPU::PUSH(RegisterPair& dest) {
    memory.write(dest.getMSB(), --SP.data);
    memory.write(dest.getLSB(), --SP.data);
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
    uint8_t data = memory.read(address);
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

    memory.write(data, address);
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
    uint8_t data = memory.read(address);
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

    memory.write(data, address);
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
    uint16_t shiftedData = memory.read(address) << 1;

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

    memory.write(shiftedData & 0xFF, address);
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
    uint8_t data = memory.read(address);

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

    memory.write(data, address);
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
    uint8_t data = memory.read(address);

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

    memory.write(data, address);
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
    uint8_t data = memory.read(address);
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

    memory.write(data, address);
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
    uint8_t data = memory.read(address);

    uint8_t low = data & 0xF;
    uint8_t high = (data & 0xF0) >> 4;

    data = high | (low << 4);

    if (data == 0) {
        setFlags(Z_FLAG);
    }
    else {
        unsetFlags(Z_FLAG);
    }

    memory.write(data, address);

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
    uint8_t data = memory.read(address);

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

    memory.write(data, address);
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
    uint8_t data = memory.read(address);

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
    uint8_t data = memory.read(address);
    data &= ~(1 << position);
    memory.write(data, address);
}

void CPU::SET(uint8_t position, Register src) {
    src.data |= 1 << position;
}

void CPU::SET(uint8_t position, uint16_t address) {
    uint8_t data = memory.read(address);
    data |= 1 << position;
    memory.write(data, address);
}
