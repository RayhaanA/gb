#include "PPU.hpp"
#include <iostream>

// todo: stat interrupt can happen when display is disabled and then re-enabled if ly == lyc (ly will equal 0 on restart)

void PPU::resetLY() {
    ly = 0;
    if (ly == lyc) {
        if (statusRegister.lyCoincidenceInterrupt) {
            requestInterrupt(STAT_INTERRUPT_FLAG);
        }
        updateCoincidenceBit(true);
    }
    else {
        updateCoincidenceBit(false);
    }
}

void PPU::incrementLY() {
    ++ly;
    if (ly < 144) {
        if (ly == lyc) {
            if (statusRegister.lyCoincidenceInterrupt) {
                requestInterrupt(STAT_INTERRUPT_FLAG);
            }
            updateCoincidenceBit(true);
        }
        else {
            updateCoincidenceBit(false);
        }
    }
}

void PPU::setDisplayBlank() {
    for (size_t i = 0; i < frameBuffer.size(); ++i) {
        frameBuffer[i] = 255;
    }
    frame.update(frameBuffer.data());
}

void PPU::tick() {
    switch (mode) {
    case PPUMode::H_BLANK:
        if (modeCycles >= H_BLANK_CYCLES) {
            modeCycles = 0;
            mode = PPUMode::OAM_SEARCH;

            if (ly == 143) {
                mode = PPUMode::V_BLANK;

                // Handle v-blank interrupt
                if (controlRegister.displayEnable) {
                    if (wroteZeroToVBLIF) {
                        wroteZeroToVBLIF = false;
                    }
                    else {
                        requestInterrupt(V_BLANK_INTERRUPT_FLAG);
                    }
                }

                // Check for stat interrupt due to v-blank
                if (statusRegister.vBlankInterrupt) {
                    requestInterrupt(STAT_INTERRUPT_FLAG);
                }

                // push frame buffer to display here
                pushFrame();
            }
            else {
                incrementLY();
            }
        }
        break;
    case PPUMode::V_BLANK:
        if (modeCycles >= V_BLANK_CYCLES) {
            modeCycles = 0;

            if (ly == 153) {
                mode = PPUMode::OAM_SEARCH;

                // Check for stat interrupt due to oam search
                if (statusRegister.oamInterrupt) {
                    requestInterrupt(STAT_INTERRUPT_FLAG);
                }

                resetLY();
            }
            else {
                incrementLY();
            }
        }
        break;
    case PPUMode::OAM_SEARCH:
        if (modeCycles >= OAM_SEARCH_CYCLES) {
            modeCycles = 0;
            mode = PPUMode::DATA_TRANSFER;
        }
        break;
    case PPUMode::DATA_TRANSFER:
        if (modeCycles == 0) {
            bgFifo = {};
            oamFifo = {};
        }
        else if (modeCycles < DATA_TRANSFER_CYCLES) {
            switch (pixelFetchState) {
            case PixelFetchState::GET_TILE:
                break;
            case PixelFetchState::GET_TILE_DATA_LOW:
                break;
            case PixelFetchState::GET_TILE_DATA_HIGH:
                break;
            case PixelFetchState::SLEEP:
                break;
            case PixelFetchState::PUSH:
                break;
            default:
                std::cout << "Unhandled pixel fetcher state " << static_cast<uint16_t>(pixelFetchState) << std::endl;
                break;
            }
        }
        else if (modeCycles >= DATA_TRANSFER_CYCLES) {
            modeCycles = 0;
            mode = PPUMode::H_BLANK;

            // Check for stat interrupt due to data transfer
            if (statusRegister.hBlankInterrupt) {
                requestInterrupt(STAT_INTERRUPT_FLAG);
            }

            // write a line to frame buffer here
            drawScanLine();
        }
        break;
    default:
        break;
    }
}

void PPU::writeRegistersValues(uint16_t address) {
    uint8_t data = (*memory)[address];
    switch (address) {
    case CONTROL_REG_ADDR:
        if (controlRegister.displayEnable && ((data & 0x80) == 0)) {
            cycles = 0;
            modeCycles = 0;
            mode = PPUMode::H_BLANK;
            ly = 0;
        }
        controlRegister.displayEnable = data & 0x80;
        controlRegister.windowTileMapSelect = data & 0x40;
        controlRegister.windowDisplayEnable = data & 0x20;
        controlRegister.bgAndWindowTileDataSelect = data & 0x10;
        controlRegister.bgTileMapDisplaySelect = data & 0x8;
        controlRegister.objectSize = data & 0x4;
        controlRegister.objectDisplayEnable = data & 0x2;
        controlRegister.bgWindowDisplayPriority = data & 0x1;
        break;
    case STATUS_REG_ADDR:
        statusRegister.lyCoincidenceInterrupt = data & 0x40;
        statusRegister.oamInterrupt = data & 0x20;
        statusRegister.vBlankInterrupt = data & 0x10;
        statusRegister.hBlankInterrupt = data & 0x8;
    case SCY_REG_ADDR:
        scy = (*memory)[address];
        break;
    case SCX_REG_ADDR:
        scx = (*memory)[address];
        break;
    case LY_REG_ADDR:
        ly = (*memory)[address];
        break;
    case LYC_REG_ADDR:
        lyc = (*memory)[address];
        if (lyc == ly) {
            if (statusRegister.lyCoincidenceInterrupt) {
                requestInterrupt(STAT_INTERRUPT_FLAG);
            }
        }
        break;
    case BGP_REG_ADDR:
        bgPalette[0] = { PALETTE[data & 0x3], data & 0x3 };
        bgPalette[1] = { PALETTE[(data & 0xC) >> 2], (data & 0xC) >> 2 };
        bgPalette[2] = { PALETTE[(data & 0x30) >> 4], (data & 0x30) >> 4 };
        bgPalette[3] = { PALETTE[(data & 0xC0) >> 6], (data & 0xC0) >> 6 };
        break;
    case OBP0_REG_ADDR:
        ob0Palette[1] = { PALETTE[(data & 0xC) >> 2], (data & 0xC) >> 2 };
        ob0Palette[2] = { PALETTE[(data & 0x30) >> 4], (data & 0x30) >> 4 };
        ob0Palette[3] = { PALETTE[(data & 0xC0) >> 6], (data & 0xC0) >> 6 };
        break;
    case OBP1_REG_ADDR:
        ob1Palette[1] = { PALETTE[(data & 0xC) >> 2], (data & 0xC) >> 2 };
        ob1Palette[2] = { PALETTE[(data & 0x30) >> 4], (data & 0x30) >> 4 };
        ob1Palette[3] = { PALETTE[(data & 0xC0) >> 6], (data & 0xC0) >> 6 };
        break;
    case WY_REG_ADDR:
        wy = (*memory)[address];
        break;
    case WX_REG_ADDR:
        wx = (*memory)[address] - WX_OFFSET;
        break;
    default:
        break;
    }
}

uint8_t PPU::readRegisterValues(uint16_t address) {
    switch (address) {
    case CONTROL_REG_ADDR:
        (*memory)[CONTROL_REG_ADDR] = (static_cast<uint8_t>(controlRegister.displayEnable) << 7) |
            (static_cast<uint8_t>(controlRegister.windowTileMapSelect) << 6) |
            (static_cast<uint8_t>(controlRegister.windowDisplayEnable) << 5) |
            (static_cast<uint8_t>(controlRegister.bgAndWindowTileDataSelect) << 4) |
            (static_cast<uint8_t>(controlRegister.bgTileMapDisplaySelect) << 3) |
            (static_cast<uint8_t>(controlRegister.objectSize) << 2) |
            (static_cast<uint8_t>(controlRegister.objectDisplayEnable) << 1) |
            static_cast<uint8_t>(controlRegister.bgWindowDisplayPriority);
        break;
    case STATUS_REG_ADDR:
        (*memory)[STATUS_REG_ADDR] = (static_cast<uint8_t>(statusRegister.lyCoincidenceInterrupt) << 6) |
            (static_cast<uint8_t>(statusRegister.oamInterrupt) << 5) |
            (static_cast<uint8_t>(statusRegister.vBlankInterrupt) << 4) |
            (static_cast<uint8_t>(statusRegister.hBlankInterrupt) << 3) |
            (static_cast<uint8_t>(statusRegister.coincideFlag) << 2) |
            static_cast<uint8_t>(mode);
        break;
    case SCY_REG_ADDR:
        (*memory)[SCY_REG_ADDR] = scy;
        break;
    case SCX_REG_ADDR:
        (*memory)[SCX_REG_ADDR] = scx;
        break;
    case LY_REG_ADDR:
        (*memory)[LY_REG_ADDR] = ly;
        break;
    case LYC_REG_ADDR:
        (*memory)[LYC_REG_ADDR] = lyc;
        break;
    case BGP_REG_ADDR:
        (*memory)[BGP_REG_ADDR] = bgPalette[0].second |
            bgPalette[1].second << 2 |
            bgPalette[2].second << 4 |
            bgPalette[3].second << 6;
        break;
    case OBP0_REG_ADDR:
        (*memory)[OBP0_REG_ADDR] =
            ob0Palette[1].second << 2 |
            ob0Palette[2].second << 4 |
            ob0Palette[3].second << 6;
        break;
    case OBP1_REG_ADDR:
        (*memory)[OBP1_REG_ADDR] =
            ob1Palette[1].second << 2 |
            ob1Palette[2].second << 4 |
            ob1Palette[3].second << 6;
        break;
    case WY_REG_ADDR:
        (*memory)[WY_REG_ADDR] = wy;
        break;
    case WX_REG_ADDR:
        (*memory)[WX_REG_ADDR] = wx + WX_OFFSET;
        break;
    default:
        break;
    }

    return (*memory)[address];  
}
