#pragma once
#include "../Globals.hpp"
#include <queue>

enum class PixelFetchState {
    GET_TILE_ID,
    GET_TILE_DATA_LOW,
    GET_TILE_DATA_HIGH,
    SLEEP,
    PUSH
};
struct Pixel {
    Color color = Color::WHITE;
    uint8_t palette = 0;
    bool bgPriority = false;
};

class PixelFetcher {
private:
    uint8_t fetcherCycles = 0;
    PixelFetchState pixelFetchState = PixelFetchState::GET_TILE_ID;

    // Fetcher data
    uint8_t fetcherCycles = 0;
    uint8_t pixelFetcherX = 0;
    uint8_t pixelFetcherY = 0;
    uint8_t tileID = 0;
    std::queue<Pixel> bgFifo;
    std::queue<Pixel> oamFifo;

public:

    void tick() {
        ++fetcherCycles;

        switch (pixelFetchState) {
            if (fetcherCycles < 2) {
                break;
            }

            fetcherCycles = 0;

            switch (pixelFetchState) {
            case PixelFetchState::GET_TILE_ID:
            {
                if (!controlRegister.bgWindowDisplayPriority) {

                }
                else {
                    uint16_t tileMapAddress = 0;
                    bool inWindow = controlRegister.windowDisplayEnable && scx >= wx;

                    if (!inWindow) {
                        tileMapAddress = mapStartValues[static_cast<uint8_t>(controlRegister.bgTileMapDisplaySelect)];
                        pixelFetcherX = (pixelFetcherX + scx / 8) & 0x1F;
                        pixelFetcherY = (ly + scy) & 0xFF;
                    }
                    else {
                        tileMapAddress = mapStartValues[static_cast<uint8_t>(controlRegister.windowTileMapSelect)];
                        pixelFetcherX = scx - wx;
                        pixelFetcherY = ly - wy;
                    }
                }


                pixelFetchState = PixelFetchState::GET_TILE_DATA_LOW;
                break;
            }
            case PixelFetchState::GET_TILE_DATA_LOW:
                pixelFetchState = PixelFetchState::GET_TILE_DATA_HIGH;
                break;
            case PixelFetchState::GET_TILE_DATA_HIGH:
                pixelFetchState = PixelFetchState::SLEEP;
                break;
            case PixelFetchState::SLEEP:
                pixelFetchState = PixelFetchState::PUSH;
                break;
            case PixelFetchState::PUSH:
                pixelFetchState = PixelFetchState::GET_TILE_ID;
                break;
            default:
                std::cout << "Unhandled pixel fetcher state " << static_cast<uint16_t>(pixelFetchState) << std::endl;
                break;
            }
        }
    }
};
