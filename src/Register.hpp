#pragma once
#include <inttypes.h>

class Register
{
private:
    uint16_t data;

public:
    void setHigh(uint8_t value);
    void setLow(uint8_t value);
    void setData(uint16_t value);
    uint8_t getHigh() const;
    uint8_t getLow() const;
    uint16_t getData() const;
};
