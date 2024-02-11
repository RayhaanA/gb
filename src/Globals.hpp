#pragma once
#include "inttypes.h"

constexpr std::size_t operator""_KB(unsigned long long v) { return 1024u * v; }

namespace Globals {
static bool running = false;
static bool skipBootRom = true;
static bool needToRunBootRom = !skipBootRom;

const size_t WIDTH = 160;
const size_t HEIGHT = 144;
const uint8_t UNDEFINED_READ = 0xFF;

const uint32_t FREQUENCY = 4194304;
const uint32_t CYCLES_PER_FRAME = 70224;
const uint8_t CYCLES_PER_INCREMENT = 4;
const uint8_t LEGACY_CYCLES_PER_INCREMENT = 4;
const uint8_t CPU_CYCLES_PER_INCREMENT = 4;

static bool IME = false;  // Interrupt Master Enable Flag: Set through op codes
static bool wroteZeroToVBLIF = false;
static bool ramEnable = false;
static bool resetSysCounter = false;

// static uint8_t currSpriteIndex = 0;
const uint8_t MAX_SPRITES = 40;
const uint16_t SPRITE_BYTE_WIDTH = 4;

constexpr uint8_t SERIAL_INTERRUPT_FLAG = 1 << 3;
}  // namespace Globals

namespace Address {
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
}  // namespace Address