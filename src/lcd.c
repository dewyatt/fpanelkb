/**
* The MIT License (MIT)
*
* Copyright (c) 2013 Daniel Wyatt
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
**/

#include "lcd.h"
#include "udelay.h"
#include <fx2regs.h>
#include <delay.h>
#include <stdarg.h>
#include <stdio.h>

#define LCD_DATA	IOC
#define LCD_DATA_OUTPUT_ENABLE OEC

#define LCD_DB7   PC7

#define LCD_RS		PA6
#define LCD_RW		PA7
#define LCD_E			PA5

#define LCD_E_ENABLE          1
#define LCD_E_DISABLE         0
#define LCD_REG_INSTRUCTION   0
#define LCD_REG_DATA          1
#define LCD_RW_READ           1
#define LCD_RW_WRITE          0

#define LCD_BUSY_FLAG     0x80

#define LCD_CLEAR           0x01

#define LCD_HOME            0x02

#define LCD_MODE_SET        0x04
#define LCD_MODE_NO_INC     0x00
#define LCD_MODE_INC        0x02
#define LCD_MODE_NO_SHIFT   0x00
#define LCD_MODE_SHIFT      0x01

#define LCD_DISPLAY         0x08
#define LCD_DISPLAY_OFF     0x08
#define LCD_DISPLAY_ON      0x0C
#define LCD_CURSOR_OFF      0x00
#define LCD_CURSOR_ON       0x02
#define LCD_BLINK_OFF       0x00
#define LCD_BLINK_ON        0x01

#define LCD_CURSOR          0x10
#define LCD_CURSOR_SHIFT    0x00
#define LCD_DISPLAY_SHIFT   0x08
#define LCD_RIGHT           0x04
#define LCD_LEFT            0x00

#define LCD_FUNCTION_SET    0x20
#define LCD_4BIT            0x00
#define LCD_8BIT            0x10
#define LCD_1_LINE          0x00
#define LCD_2_LINE          0x08
#define LCD_5X8FONT         0x00
#define LCD_5X10FONT        0x04

void lcd_cmd ( char cmd );
void lcd_cmd_nobusy ( char cmd );

void busy_wait() {
  LCD_DATA_OUTPUT_ENABLE = 0;

	LCD_RS = LCD_REG_INSTRUCTION;
	LCD_RW = LCD_RW_READ;
  do {
      LCD_E = LCD_E_DISABLE;
      LCD_E = LCD_E_ENABLE;
  }  while ( LCD_DATA & LCD_BUSY_FLAG );

  LCD_E = LCD_E_DISABLE;

  LCD_DATA_OUTPUT_ENABLE = 0xff;
}

void lcd_goto ( char x, char y ) {
  char addr = ( y == 1 ? 0x40 : 0 ) + x;
  addr |= 0x80;
  lcd_cmd ( addr );
}

void lcd_write_string ( const char *s ) {
  while (*s)
    lcd_write_char ( *s++ );
}

void lcd_write_char ( char c ) {
  busy_wait();
  LCD_DATA = c;
  LCD_RS = LCD_REG_DATA;
  LCD_RW = LCD_RW_WRITE;
  LCD_E = LCD_E_ENABLE;
  delay ( 1 );
  LCD_E = LCD_E_DISABLE;
}

void lcd_clear () {
  lcd_cmd ( LCD_CLEAR );
}

void lcd_init ( ) {
  LCD_DATA_OUTPUT_ENABLE = 0xff;
  delay ( 15 );
  lcd_cmd_nobusy ( 0x30 );
  delay ( 5 );
  lcd_cmd_nobusy ( 0x30 );
  udelay ( 100 );
  lcd_cmd_nobusy ( 0x30 );

  lcd_cmd ( LCD_FUNCTION_SET | LCD_8BIT | LCD_2_LINE | LCD_5X8FONT );
  lcd_cmd ( LCD_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF );
  lcd_cmd ( LCD_CLEAR );
}

void lcd_home () {
  lcd_cmd ( LCD_HOME );
}

void lcd_cmd_nobusy ( char cmd ) {
  LCD_RS = LCD_REG_INSTRUCTION;
  LCD_RW = LCD_RW_WRITE;
  udelay ( 40 );
  LCD_DATA = cmd;
  LCD_E = LCD_E_ENABLE;
  //udelay ( 80 );
  udelay ( 230 );
  LCD_E = LCD_E_DISABLE;
  udelay ( 10 );
}

void lcd_cmd ( char cmd ) {
  busy_wait ();
  LCD_RS = LCD_REG_INSTRUCTION;
  LCD_RW = LCD_RW_WRITE;
  udelay ( 40 );
  LCD_DATA = cmd;
  LCD_E = LCD_E_ENABLE;
  //udelay ( 80 );
  udelay ( 230 );
  LCD_E = LCD_E_DISABLE;
  udelay ( 10 );
}

