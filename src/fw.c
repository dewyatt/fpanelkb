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

#include <fx2macros.h>
#include <fx2ints.h>
#include <autovector.h>
#include <delay.h>
#include <setupdat.h>
#include "ir.h"
#include "buttons.h"

volatile __bit dosud = FALSE;
volatile __bit dosuspend = FALSE;

// custom functions
extern void main_loop();
extern void main_init();

void main() {
  main_init();

  // set up interrupts.
  USE_USB_INTS();

  ENABLE_SUDAV();
  ENABLE_USBRESET();
  ENABLE_HISPEED(); 
  ENABLE_SUSPEND();
  ENABLE_RESUME();

  EA = 1;

  // iic files (c2 load) don't need to renumerate/delay
  // trm 3.6
  #ifndef NORENUM
  RENUMERATE();
  #else
  USBCS &= ~bmDISCON;
  #endif
 
 while ( TRUE ) {
  main_loop();

  if (dosud) {
    dosud = FALSE;
    handle_setupdata();
  }

  if (dosuspend) {
    dosuspend = FALSE;
    do {
      WAKEUPCS |= bmWU | bmWU2; // make sure ext wakeups are cleared
      SUSPEND=1;
      PCON |= 1;
      __asm
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      __endasm;
    } while ( !remote_wakeup_allowed && REMOTE_WAKEUP()); 

    // resume
    // trm 6.4
    if ( REMOTE_WAKEUP() ) {
      delay ( 5 );
      USBCS |= bmSIGRESUME;
      delay ( 15 );
      USBCS &= ~bmSIGRESUME;
    }
  }

 } // end while

} // end main

void resume_isr() __interrupt RESUME_ISR {
  CLEAR_RESUME();
}

void sudav_isr() __interrupt SUDAV_ISR {
  dosud = TRUE;
  CLEAR_SUDAV();
}

void usbreset_isr() __interrupt USBRESET_ISR {
  handle_hispeed ( FALSE );
  CLEAR_USBRESET();
}

void hispeed_isr() __interrupt HISPEED_ISR {
  handle_hispeed ( TRUE );
  CLEAR_HISPEED();
}

void suspend_isr() __interrupt SUSPEND_ISR {
  dosuspend = TRUE;
  CLEAR_SUSPEND();
}

void ie0_isr() __interrupt IE0_ISR {
  ir_ie0_isr ();
}

void timer0_isr() __interrupt TF0_ISR {
  ir_timer_isr ();
}

void timer1_isr() __interrupt TF1_ISR {
  buttons_timer_isr ();
}

