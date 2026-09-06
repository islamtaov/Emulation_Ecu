/*
 * lcd_i2c.cpp
 *
 *  Created on: 14 авг. 2026 г.
 *      Author: Islam
 */

#include "lcd_i2c.hpp"
#include <cstring>
#include <cstdio>

void LCD_I2C::Init(I2C_HandleTypeDef* i2c) {
    hi2c = i2c;
    HAL_Delay(50);

    Command(0x33);
    Command(0x32);
    Command(0x28);
    Command(0x0C);
    Command(0x06);
    Command(0x01);
    HAL_Delay(10);
}

void LCD_I2C::Command(uint8_t cmd) {
    WriteByte(cmd, 0);
}

void LCD_I2C::WriteByte(uint8_t data, uint8_t rs) {
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;

    WriteNibble(high_nibble, rs);
    WriteNibble(low_nibble, rs);
}

void LCD_I2C::WriteNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = nibble | rs | LCD_BACKLIGHT;
    uint8_t data_en = data | LCD_EN;

    uint8_t buffer[4];
    buffer[0] = data_en;
    buffer[1] = data;
    buffer[2] = data_en;
    buffer[3] = data;

    HAL_I2C_Master_Transmit(hi2c, LCD_ADDR, buffer, 4, 100);
    HAL_Delay(1);
}

void LCD_I2C::Clear() {
    Command(0x01);
    HAL_Delay(5);
}

void LCD_I2C::SetCursor(uint8_t col, uint8_t row) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    address += col;
    Command(address);
}

void LCD_I2C::Print(const char* str) {
    while (*str) {
        WriteByte(*str++, 1);
    }
}

void LCD_I2C::Print(int value) {
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", value);
    Print(buf);
}


