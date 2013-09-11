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

#ifndef TIMER_H
#define TIMER_H

/*
  Macros for easier timer use. Assumes 48MHz and CLKOUT/12.

  Using too large a value should trigger a warning in SDCC about an overflow.
*/

/*
  usecs per timer tick: 1/48,000,000 * 12 * 1,000,000 = 0.25

  1     / 48,000,000 * 12                                   * 1,000,000
  (sec)   (MHz)        (timer tick is CLKOUT/12 by default)   (to microseconds)
*/

#define USECS_PER_TICK  0.25
#define USEC_TICKS(x)   (0x10000 - (WORD)((x)/USECS_PER_TICK))

#define SET_TIMER0_USEC(us)   {\
  TL0 = USEC_TICKS(us) & 0x00ff;\
  TH0 = USEC_TICKS(us) >> 8;\
}

#define RESET_TIMER0_USEC(us) {\
  TL0 = TL0 + (USEC_TICKS(us) & 0x00ff);\
  TH0 = TH0 + (USEC_TICKS(us) >> 8);\
}

#define SET_TIMER1_USEC(us)   {\
  TL1 = USEC_TICKS(us) & 0x00ff;\
  TH1 = USEC_TICKS(us) >> 8;\
}

#define RESET_TIMER1_USEC(us) {\
  TL1 = TL1 + (USEC_TICKS(us) & 0x00ff);\
  TH1 = TH1 + (USEC_TICKS(us) >> 8);\
}

#endif

