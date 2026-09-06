/*
 * emulator_app.hpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#ifndef INC_EMULATOR_APP_HPP_
#define INC_EMULATOR_APP_HPP_


#include "main.h"
#include "engine_emulator.hpp"

class EmulatorApp {
private:
    static EngineEmulator emulator;
    static EmulatorApp* instance;

public:
    static EmulatorApp& GetInstance();

    void Init();
    void Run();  // Бесконечный цикл
    void HandleTimInterrupt();

    EngineEmulator& GetEmulator() { return emulator; }
};

// Глобальная функция для C linkage (если понадобится)
extern "C" void EmulatorApp_Init(void);
extern "C" void EmulatorApp_Run(void);


#endif /* INC_EMULATOR_APP_HPP_ */
