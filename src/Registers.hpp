#pragma once
#include <inttypes.h>
#include <memory>
#include <bitset>

struct Register {
    uint8_t data = 0;

    Register();
    Register(uint8_t value);
    ~Register();

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
    RegisterPair(Register* low, Register* high);
    RegisterPair(Register&& r, Register&& l) = delete;
    RegisterPair(const RegisterPair& other);
    ~RegisterPair();

    uint16_t getData() const;
    uint8_t getMSB() const;
    uint8_t getLSB() const;

    RegisterPair& operator++() {
        uint16_t data = getData();
        ++data;

        low->data = data & 0xFF;
        high->data = data & 0xFF00;

        return *this;
    }

    RegisterPair operator++(int) {
        RegisterPair temp = *this;
        ++*this;
        return temp;
    }

    RegisterPair& operator--() {
        uint16_t data = getData();
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
    uint8_t getMSB() const;
    uint8_t getLSB() const;

    SpecialRegister();
    ~SpecialRegister();

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