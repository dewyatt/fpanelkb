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

#ifndef BUTTONS_H
#define BUTTONS_H

//7 buttons can be pressed at once
extern volatile WORD __xdata buttons[7];

//starts the timer to read buttons
void buttons_init ();

//must be called on timer1 overflows
void buttons_timer_isr ();

//In case you want to check for specific button presses
enum {
  BUTTON_FN = 1,
  BUTTON_1,
  BUTTON_2,
  BUTTON_3,
  BUTTON_4,
  BUTTON_5,
  BUTTON_6,
  BUTTON_7,
  BUTTON_8,
  BUTTON_9,
  BUTTON_10,
  BUTTON_11,
  BUTTON_12,
  BUTTON_13,
  BUTTON_14,
  BUTTON_15,
  BUTTON_16,
  //skip 1 unused
  BUTTON_RECORD = 19,
  BUTTON_NETWORK,
  BUTTON_ALARM_RESET,
  BUTTON_CANCEL,
  BUTTON_AUTO_PAN,
  BUTTON_PRESET,
  BUTTON_ZOOM_PLUS,
  BUTTON_ZOOM_MINUS,
  BUTTON_FOCUS_PLUS,
  BUTTON_FOCUS_MINUS,
  BUTTON_SPEED_PLUS,
  BUTTON_SPEED_MINUS,
  BUTTON_PTZ_UP,
  BUTTON_PTZ_LEFT,
  BUTTON_PTZ_ENTER,
  BUTTON_PTZ_RIGHT,
  BUTTON_PTZ_DOWN,
  //skip 2 unused
  BUTTON_MODE = 38,
  BUTTON_DATE,
  BUTTON_REVERSE,
  BUTTON_PAUSE,
  BUTTON_PLAY
};

#endif

