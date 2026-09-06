/*
 * engine_emulator.cpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */


#include "engine_emulator.hpp"
#include <cstdio>

// Внешние объекты из main.c
extern "C" {
    extern TIM_HandleTypeDef htim1;
    extern I2C_HandleTypeDef hi2c1;
}

EngineEmulator::EngineEmulator()
    : current_rpm(START_RPM),
      ignition_state(0),
      tooth_count(0),
      revolution_count(0) {
}

void EngineEmulator::Init() {
    lcd.Init(&hi2c1);

    encoder.Init(ENC_A_GPIO_Port, ENC_A_Pin,
                 ENC_B_GPIO_Port, ENC_B_Pin,
                 ENC_SW_GPIO_Port, ENC_SW_Pin);

    HAL_GPIO_WritePin(SW_KL15_GPIO_Port, SW_KL15_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

    lcd.Clear();
    lcd.SetCursor(0, 0);
    lcd.Print("SYSTEM READY");
    lcd.SetCursor(0, 1);
    lcd.Print("KLEMMA 15: OFF");

    SetRPM(START_RPM);
}

void EngineEmulator::SetRPM(uint32_t rpm) {
    if (rpm < MIN_RPM) rpm = MIN_RPM;
    if (rpm > MAX_RPM) rpm = MAX_RPM;

    current_rpm = rpm;

    uint32_t timer_period = 1000000 / rpm;

    if (timer_period > 10) {
        __HAL_TIM_SET_AUTORELOAD(&htim1, timer_period - 1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (timer_period / 2) - 1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 20);
    }

    if (ignition_state) {
        UpdateDisplay();
    }
}

void EngineEmulator::ToggleIgnition() {
    ignition_state = !ignition_state;

    if (ignition_state) {
        HAL_GPIO_WritePin(SW_KL15_GPIO_Port, SW_KL15_Pin, GPIO_PIN_SET);

        tooth_count = 0;
        revolution_count = 0;
        __HAL_TIM_SET_COUNTER(&htim1, 0);

        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIM_Base_Start_IT(&htim1);
    } else {
        HAL_TIM_Base_Stop_IT(&htim1);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

        HAL_GPIO_WritePin(SW_KL15_GPIO_Port, SW_KL15_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Crank_GPIO_Port, Crank_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Cam_GPIO_Port, Cam_Pin, GPIO_PIN_RESET);
    }

    UpdateDisplay();
}

void EngineEmulator::HandleToothInterrupt() {
    if (!ignition_state) return;

    tooth_count++;

    if (tooth_count >= (TEETH_TOTAL - MISSING_TEETH)) {
        tooth_count = 0;
        revolution_count++;

        if (revolution_count >= 2) {
            revolution_count = 0;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 20);
        }
    }

    if (tooth_count >= (TEETH_TOTAL - MISSING_TEETH)) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    } else {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (arr / 2) - 1);
    }
}

void EngineEmulator::Process() {
    if (encoder.IsButtonPressed()) {
        ToggleIgnition();
    }

    int32_t delta = encoder.GetDelta();
    if (delta != 0) {
        int32_t new_rpm = (int32_t)current_rpm + (delta * (int32_t)RPM_STEP);
        SetRPM((uint32_t)new_rpm);
    }
}

void EngineEmulator::UpdateDisplay() {
    lcd.SetCursor(0, 0);
    lcd.Print("RPM: ");
    lcd.Print((int)current_rpm);
    lcd.Print("    ");

    lcd.SetCursor(0, 1);
    if (ignition_state) {
        lcd.Print("KLEMMA 15: ON ");
    } else {
        lcd.Print("KLEMMA 15: OFF");
    }
}

