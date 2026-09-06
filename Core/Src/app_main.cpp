/*
 * app_main.cpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#include "emulator_app.hpp"

// Эта функция вызывается из main.c
// Но мы не трогаем main.c — используем атрибут constructor
__attribute__((constructor))
void app_init(void) {
    // Отложенная инициализация — ждём пока CubeMX всё настроит
}

// Альтернатива: используем FreeRTOS или таймер
// Самый простой способ — перехватить HAL_Init


