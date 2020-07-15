#pragma once
#include "inttypes.h"

constexpr std::size_t operator""_KB(unsigned long long v) {
    return 1024u * v;
}

const uint8_t UNDEFINED_READ = 0xFF;
const size_t MEMORY_SIZE = 64_KB;
const size_t BOOT_ROM_SIZE = 0x100;
const size_t BASE_ROM_SIZE = 32_KB;

const uint32_t FREQUENCY = 4194304;
const uint8_t CYCLES_PER_INCREMENT = 4;
static bool enableInterruptsNextCycle = false; // There's a one cycle delay on enabling IME through EI instruction
static bool timaInterruptRequest = false; // TIMA interrupt happens one cycle after overflow of TIMA   
static bool IME = false; // Interrupt Master Enable Flag: Set through op codes
static bool stopped = false;
static bool halted = false;
static bool wroteZeroToVBLIF = false;
static bool ramEnable = false;

constexpr uint8_t Z_FLAG = 1 << 7;
constexpr uint8_t N_FLAG = 1 << 6;
constexpr uint8_t H_FLAG = 1 << 5;
constexpr uint8_t C_FLAG = 1 << 4;

const uint8_t TIMER_ENABLE_BIT = 1 << 2;
const uint8_t TIMER_INPUT_CLOCK_BITS = 0x3;

constexpr uint8_t JOYPAD_INTERRUPT_FLAG = 1 << 4;
constexpr uint8_t SERIAL_INTERRUPT_FLAG = 1 << 3;
constexpr uint8_t TIMER_INTERRUPT_FLAG = 1 << 2;
constexpr uint8_t STAT_INTERRUPT_FLAG = 1 << 1;
constexpr uint8_t V_BLANK_INTERRUPT_FLAG = 1;

const uint16_t CART_HEADER_MBC = 0x147;
const uint16_t CART_HEADER_ROM_SIZE = 0x148;
const uint16_t CART_HEADER_RAM_SIZE = 0x149;

const uint16_t V_BLANK_INTERRUPT_VECTOR = 0x40;
const uint16_t LCDC_INTERRUPT_VECTOR = 0x48;
const uint16_t TIMER_INTERRUPT_VECTOR = 0x50;
const uint16_t SERIAL_INTERRUPT_VECTOR = 0x58;
const uint16_t LAST_INTERRUPT_VECTOR = 0x60;

const uint16_t INTERRUPT_VECTORS[] = { V_BLANK_INTERRUPT_VECTOR, LCDC_INTERRUPT_VECTOR, TIMER_INTERRUPT_VECTOR, SERIAL_INTERRUPT_VECTOR, LAST_INTERRUPT_VECTOR };

const uint16_t DIV_REG_ADDR = 0xFF04;
const uint16_t TIMA_REG_ADDR = 0xFF05;
const uint16_t TMA_REG_ADDR = 0xFF06;
const uint16_t TAC_REG_ADDR = 0xFF07;
const uint16_t IF_REG_ADDR = 0xFF0F;
const uint16_t IE_REG_ADDR = 0xFFFF;

const uint16_t CONTROL_REG_ADDR = 0xFF40;
const uint16_t STATUS_REG_ADDR = 0xFF41;
const uint16_t SCY_REG_ADDR = 0xFF42;
const uint16_t SCX_REG_ADDR = 0xFF43;
const uint16_t LY_REG_ADDR = 0xFF44;
const uint16_t LYC_REG_ADDR = 0xFF45;
const uint16_t BGP_REG_ADDR = 0xFF47;
const uint16_t OBP0_REG_ADDR = 0xFF48;
const uint16_t OBP1_REG_ADDR = 0xFF49;
const uint16_t WY_REG_ADDR = 0xFF4A;
const uint16_t WX_REG_ADDR = 0xFF4B;
