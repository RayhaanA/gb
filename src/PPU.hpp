#pragma once
#include <memory>
#include <unordered_map>
#include "Display.hpp"
#include "Globals.hpp"

struct ControlRegister {
    bool displayEnable = false;
    bool windowTileMapSelect = false;
    bool windowDisplayEnable = false;
    bool bgAndWindowTileDataSelect = false;
    bool bgTileMapDisplaySelect = false;
    bool objectSize = false;
    bool objectDisplayEnable = false;
    bool bgWindowDisplayPriority = false;
};

struct StatusRegister {
    bool lyCoincidenceInterrupt = false;
    bool oamInterrupt = false;
    bool vBlankInterrupt = false;
    bool hBlankInterrupt = false;
    bool coincideFlag = false;
};

enum class PPUMode {
    H_BLANK,
    V_BLANK,
    OAM_SEARCH,
    DATA_TRANSFER
};

class PPU {
    std::unique_ptr<std::vector<uint8_t>> memory;
    std::unique_ptr<Display> display;

    uint32_t cycles = 0; // Overall clock cycles
    uint32_t frameCycles = 0; // Frame cycles
    uint16_t modeCycles = 0; // Cycles in current ppu mode
    uint8_t ly = 0; // Curr scan line (Y)
    uint8_t lyc = 0; // LY compare
    uint8_t wx = 0; // Window x
    uint8_t wy = 0; // Window y
    uint8_t scx = 0; // Curr scroll x
    uint8_t scy = 0; // Curr scroll y

    const uint16_t H_BLANK_CYCLES = 200;
    const uint16_t V_BLANK_CYCLES = 456;
    const uint16_t OAM_SEARCH_CYCLES = 84;
    const uint16_t DATA_TRANSFER_CYCLES = 172;

    const uint32_t CYCLES_PER_FRAME = 70224;
    ControlRegister controlRegister;
    StatusRegister statusRegister;
    PPUMode mode = PPUMode::OAM_SEARCH;

    sf::Color TRANSPARENT = sf::Color(0, 0, 0, 0);
    std::vector<sf::Color> PALETTE = { sf::Color(255, 255, 255),
                                         sf::Color(192, 192, 192),
                                         sf::Color(96, 96, 96),
                                         sf::Color(0, 0, 0) };

    std::vector<std::pair<sf::Color, uint8_t>> bgPalette;
    std::vector<std::pair<sf::Color, uint8_t>> ob0Palette;
    std::vector<std::pair<sf::Color, uint8_t>> ob1Palette;

    std::vector<uint16_t> mapStartValues = { 0x9800, 0x9C00 }; // Window/BG tile map ranges
    std::vector<uint16_t> mapEndValues = { 0x9BFF, 0x9FFF };

    void requestInterrupt(uint8_t data) {
        (*memory)[IF_REG_ADDR] |= data & 0x1F;
    }

public:
    PPU() = default;
    explicit PPU(std::vector<uint8_t>* m, Display* d) {
        memory = std::unique_ptr<std::vector<uint8_t>>(m);
        display = std::unique_ptr<Display>(d);
        display->clearWindow();
        bgPalette = { { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
        ob0Palette = { { TRANSPARENT, 4 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
        ob1Palette = { { TRANSPARENT, 4 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
    }

    ~PPU() {
        display.release();
        memory.release();
    }

    void reset() {
        display->clearWindow();
    }
    
    void updateCoincidenceBit(bool set) {
        statusRegister.coincideFlag = set;
        if (set) {
            (*memory)[STATUS_REG_ADDR] |= 1 << 2;
        }
        else {
            (*memory)[STATUS_REG_ADDR] &= ~(1 << 2);
        }
    }

    void resetLY();
    void incrementLY();

    void tick();

    void incrementCycleCount() {
        if (controlRegister.displayEnable) {
            tick();
            cycles += CYCLES_PER_INCREMENT;
            if (cycles > FREQUENCY) {
                cycles -= FREQUENCY;
            }
            modeCycles += CYCLES_PER_INCREMENT;
        }
    }

    void updateRegistersValues(uint16_t address);

    uint8_t readRegisterValues(uint16_t address);

    PPUMode getMode() const { return mode; }
    uint32_t getModeCycles() const { return modeCycles; }
    uint8_t getLY() const { return ly; }
    uint8_t getLYC() const { return lyc; }
    uint8_t getWX() const { return wx; }
    uint8_t getWY() const { return wy; }
    uint8_t getSCY() const { return scy; }
    uint8_t getSCX() const { return scx; }
    uint8_t getLCDC() const { return (*memory)[CONTROL_REG_ADDR]; }
    uint8_t getSTAT() const { return (*memory)[STATUS_REG_ADDR]; }

    bool getDisplayEnabled() const { return controlRegister.displayEnable; }
    void write(uint8_t data, uint16_t address) { (*memory)[address] = data; }
};