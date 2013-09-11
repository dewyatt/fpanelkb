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
#include <delay.h>

/*
  4 clocks per instruction cycle

  1/FREQ * 4 (clocks per instruction cycle) => time for each cycle
  *1000000   => convert to microseconds

          microseconds per cycle
  12MHz - 0.333333333333
  24MHz - 0.166666666667
  48MHz - 0.0833333333333
*/

/*
  Insert a minimum usec delay.
  Minimum delay: ~2us
*/
void udelay (WORD usecs) {
  do {
    NOP; NOP;
  } while (--usecs != 0);
}

/*
  ; this call: 4 cycles
	mov	r6,dpl ; 2 cycles
	mov	r7,dph ; 2 cycles
;	src/udelay.c:33: do {
00101$:
;	src/udelay.c:34: NOP; NOP;
	nop ; 1 cycle
  nop ; 1 cycle
;	src/udelay.c:35: } while (--usecs != 0);
	dec	r6 ; 1 cycle
	cjne	r6,#0xFF,00110$ ; 4 cycles
	dec	r7 ; 1 cycle
00110$:
	mov	a,r6 ; 1 cycle
	orl	a,r7 ; 1 cycle
	jnz	00101$ ; 3 cycles
	ret ; 4 cycles
*/

