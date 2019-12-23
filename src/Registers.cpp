#include "Registers.hpp"

Register::Register() = default;
Register::Register(uint8_t value) : data(value) {}
Register::~Register() {}

RegisterPair::RegisterPair(Register* low, Register* high) : low(low), high(high) {}
RegisterPair::RegisterPair(const RegisterPair& other) {
    low = std::unique_ptr<Register>(new Register(other.getLSB()));
    high = std::unique_ptr<Register>(new Register(other.getMSB()));
}
RegisterPair::~RegisterPair() {
    low.release();
    high.release();
}

uint16_t RegisterPair::getData() const { return high->data << 8 | low->data; }
uint8_t RegisterPair::getMSB() const { return high->data; }
uint8_t RegisterPair::getLSB() const { return low->data; }

SpecialRegister::SpecialRegister() : data(0) {}
SpecialRegister::~SpecialRegister() = default;
uint8_t SpecialRegister::getMSB() const { return (data & 0xFF00) >> 8; }
uint8_t SpecialRegister::getLSB() const { return data & 0xFF; }