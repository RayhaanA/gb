#pragma once
#include <memory>
#include <unordered_map>
#include "Display.hpp"
#include "../Globals.hpp"
#include <iostream>
#include <queue>

enum class PPUMode {
    H_BLANK,
    V_BLANK,
    OAM_SEARCH,
    DATA_TRANSFER
};

enum class PixelFetchState {
    GET_TILE,
    GET_TILE_DATA_LOW,
    GET_TILE_DATA_HIGH,
    SLEEP,
    PUSH
};

enum class Color {
    WHITE,
    LIGHT_GREY,
    DARK_GREY,
    BLACK
};

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

struct Pixel {
    Color color = Color::WHITE;
    uint8_t palette = 0;
    bool bgPriority = false;
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
    const uint16_t WX_OFFSET = 7;

    ControlRegister controlRegister;
    StatusRegister statusRegister;
    PPUMode mode = PPUMode::OAM_SEARCH;
    PixelFetchState pixelFetchState = PixelFetchState::GET_TILE;

    uint8_t pixelFetcherX = 0;
    uint8_t pixelFetcherY = 0;

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

    std::vector<uint16_t> dataStartValues = { 0x8800, 0x8000 }; // Window/BG tile data ranges
    std::vector<uint16_t> dataEndValues = { 0x97FF, 0x8FFF };
    
    std::queue<Pixel> bgFifo;
    std::queue<Pixel> oamFifo;

    std::vector<uint8_t> frameBuffer;
    sf::Texture frame;

    bool displayIsBlank = true;

    void requestInterrupt(uint8_t data) {
        // Check to see if IRQ from previous mode triggers IRQ in following mode
        // (TCAGB 8.7)
        if (controlRegister.displayEnable) {
            (*memory)[IF_REG_ADDR] |= data & 0x1F;
        }
    }

    sf::Color getColor(Color c) {
        return PALETTE[static_cast<size_t>(c)];
    }

    void drawScanLine() {}
    void pushFrame() {}
    void resetLY();
    void incrementLY();
    void tick();
    void setDisplayBlank();

public:
    PPU() = default;
    explicit PPU(std::vector<uint8_t>* m, Display* d) {
        memory = std::unique_ptr<std::vector<uint8_t>>(m);
        display = std::unique_ptr<Display>(d);
        display->clearWindow();
        bgPalette = { { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
        ob0Palette = { { TRANSPARENT, 4 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
        ob1Palette = { { TRANSPARENT, 4 }, { PALETTE[0], 0 }, { PALETTE[0], 0 }, { PALETTE[0], 0 } };
        frameBuffer.reserve(WIDTH * HEIGHT * 4);
        for (size_t i = 0; i < frameBuffer.capacity(); ++i) {
            frameBuffer[i] = 255;
        }
        frame.create(WIDTH, HEIGHT);
        frame.update(frameBuffer.data());
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

    void incrementCycleCount() {
        if (controlRegister.displayEnable) {
            if (displayIsBlank) {
                displayIsBlank = false;
            }
            cycles += CYCLES_PER_INCREMENT;
            if (cycles > FREQUENCY) {
                cycles -= FREQUENCY;
            }
            modeCycles += CYCLES_PER_INCREMENT;
            tick();
        }
        else {
            if (!displayIsBlank) {
                setDisplayBlank();
                displayIsBlank = true;
            }
        }
    }

    void writeRegistersValues(uint16_t address);

    uint8_t readRegisterValues(uint16_t address);

    PPUMode getMode() const { return mode; }
    uint32_t getModeCycles() const { return modeCycles; }
    uint8_t getLY() { return readRegisterValues(LY_REG_ADDR); }
    uint8_t getLYC() { return readRegisterValues(LYC_REG_ADDR); }
    uint8_t getWX() { return readRegisterValues(WX_REG_ADDR); }
    uint8_t getWY() { return readRegisterValues(WY_REG_ADDR); }
    uint8_t getSCY() { return readRegisterValues(SCY_REG_ADDR); }
    uint8_t getSCX() { return readRegisterValues(SCX_REG_ADDR); }
    uint8_t getLCDC() { return readRegisterValues(CONTROL_REG_ADDR); }
    uint8_t getSTAT() { return readRegisterValues(STATUS_REG_ADDR); }
    sf::Texture& getFrame() { return frame; }

    bool getDisplayEnabled() const { return controlRegister.displayEnable; }
    void write(uint8_t data, uint16_t address) { (*memory)[address] = data; }
};