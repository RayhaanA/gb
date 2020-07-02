#pragma once
#include <inttypes.h>
#include <iostream>
#include <ios>
#include <memory>
#include <bitset>

struct Register {
    uint8_t data = 0;

    Register::Register() = default;
    Register::Register(uint8_t value) : data(value) {}
    Register::~Register() {}

    Register& operator++() {
        data++;
        return *this;
    }

    Register operator++(int) {
        Register temp = *this;
        ++*this;
        return temp;
    }

    Register& operator--() {
        data--;
        return *this;
    }

    Register operator--(int) {
        Register temp = *this;
        --*this;
        return temp;
    }

    Register& operator=(const Register& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    Register& operator=(uint8_t value) {
        data = value;
        return *this;
    }

    bool operator==(Register& other) const {
        return this == &other;
    }

    friend std::ostream& operator<<(std::ostream& os, const Register& r) {
        os << static_cast<uint16_t>(r.data) << " | " << std::hex << static_cast<uint16_t>(r.data) << std::dec
            << " | " << std::bitset <sizeof(uint8_t) * 8>(r.data) << "\n";
        return os;
    }
};

struct RegisterPair {
    std::unique_ptr<Register> low;
    std::unique_ptr<Register> high;

    RegisterPair() = delete;
    RegisterPair(Register&& r, Register&& l) = delete;

    RegisterPair::RegisterPair(Register* low, Register* high) : low(low), high(high) {}

    RegisterPair::RegisterPair(const RegisterPair& other) {
        low = std::unique_ptr<Register>(new Register(other.getLSB()));
        high = std::unique_ptr<Register>(new Register(other.getMSB()));
    }

    RegisterPair::~RegisterPair() {
        low.release();
        high.release();
    }

    inline uint16_t RegisterPair::getData() const { return high->data << 8 | low->data; }
    inline uint8_t RegisterPair::getMSB() const { return high->data; }
    inline uint8_t RegisterPair::getLSB() const { return low->data; }

    RegisterPair& operator++() {
        uint16_t data = getData();
        ++data;
        low->data = data & 0xFF;
        high->data = (data & 0xFF00) >> 8;

        return *this;
    }

    RegisterPair operator++(int) {
        RegisterPair temp = *this;
        ++*this;
        return temp;
    }

    RegisterPair& operator--() {
        uint16_t data = high->data << 8 | low->data;
        --data;

        low->data = data & 0xFF;
        high->data = (data & 0xFF00) >> 8;

        return *this;
    }

    RegisterPair operator--(int) {
        RegisterPair temp = *this;
        --*this;
        return temp;
    }

    RegisterPair& operator=(const RegisterPair& other) {
        if (this != &other) {
            low->data = other.getLSB();
            high->data = other.getMSB();
        }
        return *this;
    }

    RegisterPair& operator=(uint16_t value) {
        low->data = value & 0x00FF;
        high->data = (value & 0xFF00) >> 8;
        return *this;
    }

    bool operator==(RegisterPair& other) const {
        return this == &other;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const RegisterPair& r) {
        os << r.getData() << " | " << std::hex << r.getData() << std::dec << " | " 
            << std::bitset <sizeof(uint16_t) * 8>(r.getData()) << "\n";
        return os;
    }
};

struct SpecialRegister {
    uint16_t data;

    SpecialRegister::SpecialRegister() : data(0) {}
    SpecialRegister::~SpecialRegister() = default;
    uint8_t SpecialRegister::getMSB() const { return (data & 0xFF00) >> 8; }
    uint8_t SpecialRegister::getLSB() const { return data & 0xFF; }

    SpecialRegister& operator++() {
        ++data;
        return *this;
    }

    SpecialRegister operator++(int) {
        SpecialRegister temp = *this;
        ++*this;
        return temp;
    }

    SpecialRegister& operator--() {
        --data;
        return *this;
    }

    SpecialRegister operator--(int) {
        SpecialRegister temp = *this;
        --*this;
        return temp;
    }

    SpecialRegister& operator=(const SpecialRegister& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    SpecialRegister& operator=(uint16_t value) {
        data = value;
        return *this;
    }

    bool operator==(SpecialRegister& other) const {
        return this == &other;
    }

    friend std::ostream& operator<<(std::ostream& os, const SpecialRegister& r) {
        os << r.data << " | " << std::hex << r.data << std::dec << " | " 
            << std::bitset <sizeof(uint16_t) * 8>(r.data) << "\n";
        return os;
    }
};