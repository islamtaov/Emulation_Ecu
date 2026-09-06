/*
 * encoder.cpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */


#include "encoder.hpp"

void Encoder::Init(GPIO_TypeDef* gpio_a, uint16_t a,
                   GPIO_TypeDef* gpio_b, uint16_t b,
                   GPIO_TypeDef* gpio_btn, uint16_t btn) {
    port_a = gpio_a;
    port_b = gpio_b;
    port_btn = gpio_btn;
    pin_a = a;
    pin_b = b;
    pin_btn = btn;

    last_a = HAL_GPIO_ReadPin(port_a, pin_a);
    last_btn = HAL_GPIO_ReadPin(port_btn, pin_btn);
    last_debounce = HAL_GetTick();
}

int32_t Encoder::GetDelta() {
    uint8_t current_a = HAL_GPIO_ReadPin(port_a, pin_a);
    int32_t delta = 0;

    if (current_a != last_a) {
        if (current_a == GPIO_PIN_RESET) {
            uint8_t current_b = HAL_GPIO_ReadPin(port_b, pin_b);
            if (current_b == GPIO_PIN_SET) {
                delta = 1;
            } else {
                delta = -1;
            }
        }
        last_a = current_a;
    }

    return delta;
}

bool Encoder::IsButtonPressed() {
    uint8_t current_btn = HAL_GPIO_ReadPin(port_btn, pin_btn);
    uint32_t now = HAL_GetTick();
    bool pressed = false;

    if (current_btn == GPIO_PIN_RESET && last_btn == GPIO_PIN_SET) {
        if ((now - last_debounce) > 50) {
            last_debounce = now;
            pressed = true;
        }
    }

    last_btn = current_btn;
    return pressed;
}

