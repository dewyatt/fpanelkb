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

#include <fx2types.h>
#include <fx2regs.h>
#include <fx2ints.h>
#include "timer.h"

volatile WORD __xdata buttons[7] = {0};

void buttons_init () {
  //timer1 16-bit mode
  TMOD &= ~0xf0;
  TMOD |= 0x10;
  //enable timer1 interrupt
  ENABLE_TIMER1();

  //5ms
  //This takes care of debouncing.
  SET_TIMER1_USEC(1000 * 5);
  //start timer1
  TR1 = 1;
}

WORD make_keycode ( WORD e, WORD r ) {
  switch ( e ) {
    case 0x7C:
      return r * 6 + 1;
    case 0xBC:
      return r * 6 + 2;
    case 0xDC:
      return r * 6 + 3;
    case 0xEC:
      return r * 6 + 4;
    case 0xF4:
      return r * 6 + 5;
    case 0xF8:
      return r * 6 + 6;
  }
  return 0;
}

//Fills buttons[7] array with any pressed keys
void scan_buttons () {
  int i;
  for ( i = 0; i < 5; i++ ) {
    //overly complex version:
    //IOD = ( IOD & 0xE0 ) | (0x1F & (~(1 << i)));
    IOD |= 0x1F; //Turn on all bits except top 3 (LED outputs)
    IOD &= ~(1 << i); //Turn off column i
    IOE = 0xFF;
    buttons[i] = make_keycode ( IOE & 0xFC, i );
  }
  IOD |= 0x1F;
  IOE = 0xFE;
  buttons[5] = make_keycode ( IOE & 0xFC, 5 );
  IOD |= 0x1F;
  IOE = 0xFD;
  buttons[6] = make_keycode ( IOE & 0xFC, 6 );
  IOE = 0xFF;
}

void buttons_timer_isr () {
  scan_buttons();
}

