#pragma once
#include "inttypes.h"

static bool running = false;
static bool skipBootRom = false;
static bool needToRunBootRom = !skipBootRom;

const uint8_t BOOT_ROM[256] = {
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x00, 0x00, 0x23, 0x7D, 0xFE, 0x34, 0x20,
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x00, 0x00, 0x3E, 0x01, 0xE0, 0x50
};

constexpr std::size_t operator""_KB(unsigned long long v) {
    return 1024u * v;
}

const size_t WIDTH = 160;
const size_t HEIGHT = 144;
const uint8_t UNDEFINED_READ = 0xFF;
const size_t MEMORY_SIZE = 64_KB;
const size_t BOOT_ROM_SIZE = 0x100;
const size_t BASE_ROM_SIZE = 32_KB;
const size_t START_OF_ROM = 0x100;

const uint32_t FREQUENCY = 4194304;
const uint32_t CYCLES_PER_FRAME = 70224;
const uint8_t CYCLES_PER_INCREMENT = 4;

static bool IME = false; // Interrupt Master Enable Flag: Set through op codes
static bool wroteZeroToVBLIF = false;
static bool ramEnable = false;
static bool resetSysCounter = false;

static uint8_t currSpriteIndex = 0;
const uint8_t MAX_SPRITES = 40;
const uint16_t SPRITE_BYTE_WIDTH = 4;

constexpr uint8_t Z_FLAG = 1 << 7;
constexpr uint8_t N_FLAG = 1 << 6;
constexpr uint8_t H_FLAG = 1 << 5;
constexpr uint8_t C_FLAG = 1 << 4;

constexpr uint8_t TIMER_ENABLE_BIT = 1 << 2;
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

const uint16_t OAM_TABLE_ADDR = 0xFE00;
const uint16_t JOYPAD_REG_ADDR = 0xFF00;
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
const uint16_t DMA_TRANSFER_ADDR = 0xFF46;
const uint16_t BGP_REG_ADDR = 0xFF47;
const uint16_t OBP0_REG_ADDR = 0xFF48;
const uint16_t OBP1_REG_ADDR = 0xFF49;
const uint16_t WY_REG_ADDR = 0xFF4A;
const uint16_t WX_REG_ADDR = 0xFF4B;
