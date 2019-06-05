#include "Register.hpp"

void Register::setHigh(uint8_t value) 
{ 
    data |= value << 8; 
}

void Register::setLow(uint8_t value)
{
    data |= value;
}

void Register::setData(uint16_t value)
{
    data |= value;
}

uint8_t Register::getHigh() const
{
    return data & 0xFF00;
}

uint8_t Register::getLow() const
{
    return data & 0x00FF;
}

uint16_t Register::getData() const
{
    return data;
}
