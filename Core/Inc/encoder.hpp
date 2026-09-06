/*
 * encoder.hpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#ifndef INC_ENCODER_HPP_
#define INC_ENCODER_HPP_

#ifdef __cplusplus

#include "main.h"

class Encoder {
private:
    GPIO_TypeDef* port_a;
    GPIO_TypeDef* port_b;
    GPIO_TypeDef* port_btn;
    uint16_t pin_a;
    uint16_t pin_b;
    uint16_t pin_btn;

    uint8_t last_a;
    uint8_t last_btn;
    uint32_t last_debounce;

public:
    Encoder() : port_a(nullptr), port_b(nullptr), port_btn(nullptr),
                pin_a(0), pin_b(0), pin_btn(0),
                last_a(1), last_btn(1), last_debounce(0) {}

    void Init(GPIO_TypeDef* gpio_a, uint16_t a,
              GPIO_TypeDef* gpio_b, uint16_t b,
              GPIO_TypeDef* gpio_btn, uint16_t btn);

    int32_t GetDelta();
    bool IsButtonPressed();
};

#endif

#endif /* INC_ENCODER_HPP_ */
