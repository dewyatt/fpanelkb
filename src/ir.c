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

#include <fx2regs.h>
#include <fx2ints.h>
#include <delay.h>
#include <string.h>
#include "ir.h"
#include "timer.h"

#define IR_PORT   PA0

#define MARK                    0
#define SPACE                   1

//444us
#define RC6_UNIT                444

//array for 58 IR samples (wasteful on space, but faster than a bit array).
//not __xdata for better performance...
volatile unsigned char irbuffer[58];
volatile unsigned char count;
volatile BOOL got_ir;

BOOL decode_bit ( BOOL *bit, unsigned char a1, unsigned char a2 ) {
  if ( a1 == a2 )
    return FALSE;

  *bit = (a1 == MARK);
  return TRUE;
}

BOOL decode_ir ( WORD *mode, BOOL *toggle, WORD *addr, WORD *cmd ) {
  int i;
  BOOL bit;
  if (!got_ir)
    return FALSE;

  //leader mark
  for ( i = 0; i < 6; i++ )
    if ( irbuffer[i] != MARK )
      return FALSE;

  //leader space
  if ( irbuffer[6] != SPACE || irbuffer[7] != SPACE )
    return FALSE;

  //start bit
  if ( !decode_bit ( &bit, irbuffer[8], irbuffer[9] ) || bit != TRUE )
    return FALSE;

  //mode
  *mode = 0;
  for ( i = 0; i < 3; i++ ) {
    if ( !decode_bit ( &bit, irbuffer[10 + i * 2], irbuffer[10 + i * 2 + 1] ) )
      return FALSE;

    *mode <<= 1;
    if ( bit )
      *mode |= 1;
  }

  //toggle/trailer
  if ( irbuffer[16] != irbuffer[17] || irbuffer[18] != irbuffer[19] )
    return FALSE;

  *toggle = ( irbuffer[16] == MARK );

  //addr
  *addr = 0;
  for ( i = 0; i < 8; i++ ) {
    if ( !decode_bit ( &bit, irbuffer[20 + i * 2], irbuffer[20 + i * 2 + 1] ) )
      return FALSE;

    *addr <<= 1;
    if ( bit )
      *addr |= 1;
  }

  //cmd
  *cmd = 0;
  for ( i = 0; i < 8; i++ ) {
    if ( !decode_bit ( &bit, irbuffer[36 + i * 2], irbuffer[36 + i * 2 + 1] ) )
      return FALSE;

    *cmd <<= 1;
    if ( bit )
      *cmd |= 1;
  }

  //signal free time
  for ( i = 0; i < 6; i++ ) {
    if ( irbuffer[52 + i] == MARK )
      return FALSE;
  }
  return TRUE;
}

void ir_init () {
  //timer0 16-bit mode
  TMOD &= ~0x0f;
  TMOD |= 1;
  //enable timer0 interrupt, don't start timer yet
  ENABLE_TIMER0();
  PT0 = 1;

  //interrupt when PA0 falls
  IT0 = 0;
  got_ir = FALSE;
}

void ir_start () {
  got_ir = FALSE;
  EX0 = 1;
}

//Called when PA0 falls, indicating IR light is present.
//Start the decoding state machine, timer, etc, disable the interrupt.
void ir_ie0_isr () {
  //Disable this interrupt (we only use it as a starting point)
  EX0 = 0;

  count = 0;
  got_ir = FALSE;
  //jump in part of the way
  SET_TIMER0_USEC(RC6_UNIT/4);
  //start timer0
  TR0 = 1;
}

void ir_timer_isr () {
  unsigned char data = IR_PORT;
  TR0 = 0;

  irbuffer[count] = data;
  count++;
  if ( count != 58 ) {
    RESET_TIMER0_USEC(RC6_UNIT);
    TR0 = 1;
  } else
    got_ir = TRUE;
}

