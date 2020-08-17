#pragma once
#include <memory>
#include <unordered_map>
#include "Display.hpp"
#include "../Globals.hpp"
#include <iostream>
#include <deque>

enum class PPUMode {
    H_BLANK,
    V_BLANK,
    OAM_SEARCH,
    DATA_TRANSFER
};

enum class PixelFetcherState {
    GET_TILE_ID,
    GET_TILE_DATA_LOW,
    GET_TILE_DATA_HIGH,
    SLEEP,
    PUSH,
    GET_SPRITE_TILE_ID,
    GET_SPRITE_FLAGS,
    GET_SPRITE_DATA_LOW,
    GET_SPRITE_DATA_HIGH,
    PUSH_SPRITE
};

enum class Color {
    WHITE,
    LIGHT_GREY,
    DARK_GREY,
    BLACK
};

enum class PaletteID {
    BGP,
    OBP0,
    OBP1
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
    uint8_t color = 0;
    PaletteID palette = PaletteID::BGP;
    bool bgPriority = false;
    Pixel() {
        color = 0, palette = PaletteID::BGP, bgPriority = false;
    }
    Pixel(uint8_t c, uint8_t p, bool b) : color(c), palette(static_cast<PaletteID>(p)), bgPriority(b) {}
};

struct Sprite {
    uint8_t y = 0;
    uint8_t x = 0;
    uint8_t tile = 0;
    uint8_t attributes = 0;
    uint8_t index = 0;

    Sprite() {
        y = 0;
        x = 0;
        tile = 0;
        attributes = 0;
        index = 0;
    }

    Sprite(uint8_t ypos, uint8_t xpos, uint8_t t, uint8_t a, uint8_t i) :
        y(ypos), x(xpos), tile(t), attributes(a), index(i) {}

    // Sprite priority is based on lower x values having higher priority
    // Equal x means sprite which comes first in memory has higher priority
    bool operator<(const Sprite& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        else {
            return index < other.index;
        }
    }

    bool hasPriorityOverBg() { return !(attributes & 0x80); }
    bool yFlip() { return attributes & 0x40; }
    bool xFlip() { return attributes & 0x20; }
    bool paletteNumber() { return attributes & 0x10; }
};

class PPU {
    std::unique_ptr<std::vector<uint8_t>> memory;
    std::unique_ptr<Display> display;

    uint16_t modeCycles = 0; // Cycles in current ppu mode
    uint8_t ly = 0; // Curr scan line (Y)
    uint8_t lyc = 0; // LY compare
    uint8_t wx = 0; // Window x
    uint8_t wy = 0; // Window y
    uint8_t scx = 0; // Curr scroll x
    uint8_t scy = 0; // Curr scroll y

    // H-blank and data transfer can be variable length depending on number of sprites and 
    // how much fifos need to fill up. They default to the given numbers
    uint16_t H_BLANK_CYCLES = 204;
    uint16_t DATA_TRANSFER_CYCLES = 172;
    uint16_t ACTUAL_DATA_TRANSFER_CYCLES_FOR_LINE = 0;
    uint16_t ACTUAL_H_BLANK_CYCLES_FOR_LINE = 204;

    const uint16_t V_BLANK_CYCLES = 456;
    const uint16_t OAM_SEARCH_CYCLES = 80;
    const uint16_t WX_OFFSET = 7;
    const uint16_t SPRITE_Y_OFFSET = 16;
    const uint16_t SPRITE_X_OFFSET = 8;
    const uint16_t MAX_SPRITES_PER_LINE = 10;

    bool finishedRenderingLine = false;
    bool haventDrawnYet = true;
    bool dontDrawFirstFrame = false;

    ControlRegister controlRegister;
    StatusRegister statusRegister;
    PPUMode mode = PPUMode::OAM_SEARCH;

    // Drawing x pos
    uint8_t x = 0;

    // Window active
    bool window = false;

    // Fetcher data
    PixelFetcherState fetcherState = PixelFetcherState::GET_TILE_ID;
    uint8_t fetcherCycles = 2;
    uint8_t tileID = 0;
    uint16_t mapAddress = 0;
    uint16_t tileDataAddress = 0;
    uint8_t xOffset = 0;
    bool signedTileID = false;
    uint8_t tileLine = 0;
    std::deque<Pixel> bgFifo;
    uint8_t tileDataLow = 0;
    uint8_t tileDataHigh = 0;
    bool disableFetcher = false;
    uint8_t skippedPixels = 0;
    Sprite currSprite;
    uint8_t spriteTileLine = 0;
    int16_t spriteOffset = 0;
    uint8_t spriteIndex = 0;
    uint8_t spriteAttributes = 0;

    // Sprite look up
    std::vector<Sprite> spritesForCurrLine;
    uint8_t currSpriteIndex = 0;

    // Pallettes
    sf::Color TRANSPARENT = sf::Color(0, 0, 0, 0);
    std::vector<sf::Color> PALETTE = { sf::Color(255, 255, 255),
                                         sf::Color(192, 192, 192),
                                         sf::Color(96, 96, 96),
                                         sf::Color(0, 0, 0) };

    std::vector<std::pair<sf::Color, uint8_t>> bgPalette;
    std::vector<std::pair<sf::Color, uint8_t>> ob0Palette;
    std::vector<std::pair<sf::Color, uint8_t>> ob1Palette;

    // Tile map addresses
    std::vector<uint16_t> mapStartValues = { 0x9800, 0x9C00 }; // Window/BG tile map ranges

    // VRAM key locations
    std::vector<uint16_t> dataStartValues = { 0x8800, 0x8000 }; // Window/BG tile data ranges

    // Frame to draw on
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

    sf::Color getColor(Pixel& p) {
        switch (p.palette) {
        case PaletteID::BGP:
            return bgPalette[p.color].first;
            break;
        case PaletteID::OBP0:
            return ob0Palette[p.color].first;
            break;
        case PaletteID::OBP1:
            return ob1Palette[p.color].first;
            break;
        default:
            return TRANSPARENT;
            break;
        }
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

    bool spriteIsVisible(uint16_t spriteAddress) {
        uint8_t y = (*memory)[spriteAddress];
        uint8_t x = (*memory)[spriteAddress + 1];
        
        return y <= (ly + SPRITE_Y_OFFSET) &&
            (ly + SPRITE_Y_OFFSET) < (y + (controlRegister.objectSize ? 16 : 8)) &&
            x != 0;
    }

    Sprite createSprite(uint16_t address, uint8_t index) {
        return { static_cast<uint8_t>((*memory)[address]), static_cast<uint8_t>((*memory)[address + 1]),
            (*memory)[address + 2], (*memory)[address + 3], index };
    }

    // Fetcher functions
    void tickFetcher();
    void addSpriteToFetcher(Sprite& s, int16_t offset);
    uint8_t readTileData(uint8_t tileId, uint8_t line, uint8_t byteNumber, uint16_t tileDataAddress, 
        bool signedID, uint8_t attributes, uint8_t tileHeight);

    void drawBlankLine();
    void drawScanLine();
    void drawSprites();
    void pushFrame() { frame.update(frameBuffer.data()); }
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
            frameBuffer.push_back(255);
        }
        frame.create(WIDTH, HEIGHT);
        frame.update(frameBuffer.data());
        spritesForCurrLine.reserve(MAX_SPRITES_PER_LINE);
    }

    ~PPU() {
        display.release();
        memory.release();
    }

    void reset() {
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
        modeCycles = 0;
        ly = 0;
        lyc = 0;
        wx = 0;
        wy = 0;
        scx = 0;
        scy = 0;
        controlRegister = {};
        statusRegister = {};
        mode = PPUMode::OAM_SEARCH;
    }

    void incrementCycleCount() {
        if (controlRegister.displayEnable) {
            if (displayIsBlank) {
                displayIsBlank = false;
            }
            tick();
            modeCycles += CYCLES_PER_INCREMENT;
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
    uint8_t getCurrSpriteIndex() { return currSpriteIndex; }

    bool getDisplayEnabled() { return (getLCDC() & 0x80); }
    void write(uint8_t data, uint16_t address) { (*memory)[address] = data; }
};