/*
 * lcd_i2c.hpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#ifndef INC_LCD_I2C_HPP_
#define INC_LCD_I2C_HPP_


#include "main.h"

class LCD_I2C {
private:
    I2C_HandleTypeDef* hi2c;
    static const uint8_t LCD_ADDR = 0x27 << 1;
    static const uint8_t LCD_RS = 0x01;
    static const uint8_t LCD_EN = 0x04;
    static const uint8_t LCD_BACKLIGHT = 0x08;

    void WriteNibble(uint8_t nibble, uint8_t rs);
    void WriteByte(uint8_t data, uint8_t rs);
    void Command(uint8_t cmd);

public:
    LCD_I2C() : hi2c(nullptr) {}
    void Init(I2C_HandleTypeDef* i2c);
    void Clear();
    void SetCursor(uint8_t col, uint8_t row);
    void Print(const char* str);
    void Print(int value);
};


#endif /* INC_LCD_I2C_HPP_ */
