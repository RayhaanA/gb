#pragma once
#include "../Globals.hpp"

class Timers {
    std::unique_ptr<uint8_t> divHigh; // div high holds the actual value of the timer
    std::unique_ptr<uint8_t> divLow;
    std::unique_ptr<uint8_t> tima;
    std::unique_ptr<uint8_t> tma;
    std::unique_ptr<uint8_t> tac;

    uint32_t divCycleCount = 0;
    uint32_t timerCycleCount = 0;
    const std::vector<uint32_t> timaIncrementRate = { FREQUENCY / 1024, FREQUENCY / 16, FREQUENCY / 64, FREQUENCY / 256 };
    const uint32_t divIncrementRate = FREQUENCY / 256;

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

    // Handle DIV and TIMA register functioning; todo: bunch of bugs related to this in gb system, does it even work regardless of bugs?
    void incrementTIMA() {
        if (*tac & TIMER_ENABLE_BIT) {
            if (timerCycleCount == timaIncrementRate[*tac & TIMER_INPUT_CLOCK_BITS]) {
                if (*tima == std::numeric_limits<uint8_t>::max()) {
                    *tima = 0;
                    timaInterruptRequest = true;
                }
                else {
                    *tima = *tima + 1;
                }
                timerCycleCount = 0;
            }
            timerCycleCount += CYCLES_PER_INCREMENT;
        }
    }

    void incrementTimers() {
        if (divCycleCount > divIncrementRate) {
            *divHigh = *divHigh + 1;
            divCycleCount = 0;
        }
        divCycleCount += CYCLES_PER_INCREMENT;
        incrementTIMA();
    }
};