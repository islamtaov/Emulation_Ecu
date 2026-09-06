/*
 * engine_emulator.hpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#ifndef INC_ENGINE_EMULATOR_HPP_
#define INC_ENGINE_EMULATOR_HPP_

#ifdef __cplusplus

#include "main.h"
#include "lcd_i2c.hpp"
#include "encoder.hpp"

class EngineEmulator {
private:
    static const uint32_t MIN_RPM = 200;
    static const uint32_t MAX_RPM = 6000;
    static const uint32_t START_RPM = 800;
    static const uint32_t RPM_STEP = 50;
    static const uint32_t TEETH_TOTAL = 60;
    static const uint32_t MISSING_TEETH = 2;

    volatile uint32_t current_rpm;
    volatile uint8_t ignition_state;
    volatile uint32_t tooth_count;
    volatile uint32_t revolution_count;

    LCD_I2C lcd;
    Encoder encoder;

    void UpdateDisplay();

public:
    EngineEmulator();
    void Init();
    void Process();
    void SetRPM(uint32_t rpm);
    void ToggleIgnition();
    void HandleToothInterrupt();

    uint32_t GetRPM() const { return current_rpm; }
    uint8_t GetIgnitionState() const { return ignition_state; }
};

#endif

#endif /* INC_ENGINE_EMULATOR_HPP_ */
