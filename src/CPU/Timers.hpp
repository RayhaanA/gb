#pragma once
#include "../Globals.hpp"

class Timers {
    std::unique_ptr<uint8_t> divHigh; // div high holds the actual value of the timer
    std::unique_ptr<uint8_t> divLow;
    std::unique_ptr<uint8_t> tima;
    std::unique_ptr<uint8_t> tma;
    std::unique_ptr<uint8_t> tac;

    uint16_t divCounter = 0;
    uint32_t timerCounter = 0;
    const std::vector<uint32_t> timaIncrementRate = { FREQUENCY / 4096, FREQUENCY / 262144, FREQUENCY / 65536, FREQUENCY / 16386 };
    const uint32_t divOverflowPeriod = FREQUENCY / 64;

    std::unique_ptr<std::vector<uint8_t>> memory;

public:
    explicit Timers(std::vector<uint8_t>* m) {
        memory = std::unique_ptr<std::vector<uint8_t>>(m);

        divHigh = std::unique_ptr<uint8_t>(&(*m)[DIV_REG_ADDR]);
        divLow = std::unique_ptr<uint8_t>(&(*m)[DIV_REG_ADDR - 1]);
        tima = std::unique_ptr<uint8_t>(&(*m)[TIMA_REG_ADDR]);
        tma = std::unique_ptr<uint8_t>(&(*m)[TMA_REG_ADDR]);
        tac = std::unique_ptr<uint8_t>(&(*m)[TAC_REG_ADDR]);
    }

    ~Timers() {
        divHigh.release();
        divLow.release();
        tima.release();
        tma.release();
        tac.release();
        memory.release();
    }

    void incrementDIV() {
        if (resetSysCounter) {
            divCounter = 0;
            resetSysCounter = false;
        }

        divCounter += CYCLES_PER_INCREMENT;
        *divLow = divCounter & 0xFF;
        *divHigh = (divCounter & 0xFF00) >> 8;
    }

    // Handle DIV and TIMA register functioning; todo: bunch of bugs related to this in gb system, does it even work regardless of bugs?
    void incrementTIMA() {
        if (*tac & TIMER_ENABLE_BIT) {
            if (timerCounter >= timaIncrementRate[*tac & TIMER_INPUT_CLOCK_BITS]) {
                if (*tima == std::numeric_limits<uint8_t>::max()) {
                    *tima = 0;
                    timaInterruptRequest = true;
                }
                else {
                    *tima = *tima + 1;
                }
                timerCounter = 0;
            }
            timerCounter += CYCLES_PER_INCREMENT;
        }
    }

    void incrementTimers() {
        incrementDIV();
        incrementTIMA();
    }

    void reset() {
        divCounter = 0;
        resetSysCounter = false;
        timerCounter = 0;
    }
};