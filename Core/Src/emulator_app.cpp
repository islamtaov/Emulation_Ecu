/*
 * emulator_app.cpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */


#include "emulator_app.hpp"

// Внешние объекты из main.c
extern "C" {
    extern TIM_HandleTypeDef htim1;
}

EngineEmulator EmulatorApp::emulator;
EmulatorApp* EmulatorApp::instance = nullptr;

EmulatorApp& EmulatorApp::GetInstance() {
    if (instance == nullptr) {
        instance = new EmulatorApp();
    }
    return *instance;
}

void EmulatorApp::Init() {
    emulator.Init();
}

void EmulatorApp::Run() {
    while (1) {
        emulator.Process();
        HAL_Delay(10);
    }
}

void EmulatorApp::HandleTimInterrupt() {
    emulator.HandleToothInterrupt();
}

// C linkage функции
extern "C" void EmulatorApp_Init(void) {
    EmulatorApp::GetInstance().Init();
}

extern "C" void EmulatorApp_Run(void) {
    EmulatorApp::GetInstance().Run();
}

