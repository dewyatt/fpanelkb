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
#include <eputils.h>
#include "lcd.h"
#include "ir.h"
#include "buttons.h"
#include "hidkeys.h"

#define DSCR_HID          0x21
#define DSCR_HID_REPORT   0x22

extern __code WORD hid_dscr;
extern __code WORD hid_report_dscr;
extern __code WORD hid_report_dscr_end;

BOOL handle_get_descriptor() {
	WORD length;//, i;
  switch ( SETUPDAT[3] ) {
    case DSCR_HID:
      SUDPTRH = MSB((WORD)&hid_dscr);
      SUDPTRL = LSB((WORD)&hid_dscr);
      return TRUE;
    case DSCR_HID_REPORT:
      length = ((WORD)&hid_report_dscr_end) - ((WORD)&hid_report_dscr);
      //handles packets > 64 bytes
      writeep0((BYTE*)&hid_report_dscr, length);
      return TRUE;
  }
  return FALSE;
}

//************************** Configuration Handlers *****************************

// set *alt_ifc to the current alt interface for ifc
BOOL handle_get_interface(BYTE ifc, BYTE* alt_ifc) {
  ifc;
  alt_ifc;
  return TRUE;
}
// return TRUE if you set the interface requested
// NOTE this function should reconfigure and reset the endpoints
// according to the interface descriptors you provided.
BOOL handle_set_interface(BYTE ifc,BYTE alt_ifc) {  
  ifc;
  alt_ifc;
  return TRUE;
}

// handle getting and setting the configuration
// 1 is the default.  If you support more than one config
// keep track of the config number and return the correct number
// config numbers are set int the dscr file.
//volatile BYTE config=1;
BYTE handle_get_configuration() { 
  return 1;
}

// NOTE changing config requires the device to reset all the endpoints
BOOL handle_set_configuration(BYTE cfg) { 
  cfg;
  return TRUE;
}


//******************* VENDOR COMMAND HANDLERS **************************


BOOL handle_vendorcommand(BYTE cmd) {
  cmd;
  return FALSE;
}

//We only support 1 button press at a time.
//7 button presses can actually be detected at once.
WORD get_button () {
  int i;
//  scan_buttons ();
  for ( i = 0; i < 7; i++ ) {
    if ( buttons[i] )
      return buttons[i];
  }
  return 0;
}

__xdata const WORD button_keymap[] = {
  0, //unused

  KEYBOARD_CAPS_LOCK, //FN
  KEYBOARD_A, //1
  KEYBOARD_B, //2
  KEYBOARD_C, //3
  KEYBOARD_D, //4
  KEYBOARD_E, //5
  KEYBOARD_F, //6
  KEYBOARD_G, //7
  KEYBOARD_H, //8
  KEYBOARD_I, //9
  KEYBOARD_J, //10
  KEYBOARD_K, //11
  KEYBOARD_L, //12
  KEYBOARD_M, //13
  KEYBOARD_N, //14
  KEYBOARD_O, //15
  KEYBOARD_P, //16

  0, //unused

  0, //record
  0, //network
  0, //alarm reset
  KEYBOARD_ESCAPE, //cancel
  0, //autopan
  0, //preset
  0, //zoom+
  0, //zoom-
  0, //focus+
  0, //focus-
  0, //speed+
  0, //speed-
  KEYBOARD_UP_ARROW, //ptz up
  KEYBOARD_LEFT_ARROW, //ptz left
  KEYBOARD_ENTER, //ptz enter
  KEYBOARD_RIGHT_ARROW, //ptz right
  KEYBOARD_DOWN_ARROW, //ptz down

  0, //unused
  0, //unused

  0, //mode
  0, //date
  0, //reverse
  KEYBOARD_SPACEBAR, //pause
  KEYBOARD_SPACEBAR, //play
};

//We use a function rather than an array since these go up kind of high.
WORD remote_key_map ( WORD button ) {
  switch ( button ) {
    case 0x58: //up
      return KEYBOARD_UP_ARROW;
    case 0x59: //down
      return KEYBOARD_DOWN_ARROW;
    case 0x5a: //left
      return KEYBOARD_LEFT_ARROW;
    case 0x5b: //right
      return KEYBOARD_RIGHT_ARROW;
    case 0x5c: //ok
      return KEYBOARD_ENTER;
    case 0x30: //pause
      return KEYBOARD_SPACEBAR;
    case 0x31: //stop
      return KEYBOARD_X;
    case 0x29: //rewind
      return KEYBOARD_R;
    case 0x28: //fast forward
      return KEYBOARD_F;
    case 0x2c: //play
      return KEYBOARD_SPACEBAR;
    case 0x0f: //display
      return KEYBOARD_I;
    case 0x0c: //power
      return KEYBOARD_S;
    case 0x4b: //subtitle
      return KEYBOARD_L;
    case 0x83: //back
      return KEYBOARD_BACKSPACE;
    case 0xf7: //zoom
      return KEYBOARD_Z;
    case 0xc9: //title
      return KEYBOARD_O;
  }
  return 0;
}

//Yes, these are in an unintuitive order.
//CW = clockwise
//CCW = counter-clockwise
//There is 1 resting position, 7 CW, and 8 CCW. CCW 1 is better ignored.
__xdata const WORD shuttle_keymap[] = {
  KEYBOARD_H, //no rotation
  0x00, //CCW 1 (we ignore this one so there are 7 CW and 7 CCW)

                        KEYBOARD_K, //CW  3
  KEYBOARD_E, //CCW 4
                        KEYBOARD_I, //CW  1
  KEYBOARD_G, //CCW 2
                        KEYBOARD_J, //CW  2
  KEYBOARD_F, //CCW 3
                        KEYBOARD_O, //CW  7
  KEYBOARD_A, //CCW 8
                        KEYBOARD_L, //CW  4
  KEYBOARD_D, //CCW 5
                        KEYBOARD_N, //CW  6
  KEYBOARD_B, //CCW 7
                        KEYBOARD_M, //CW  5
  KEYBOARD_C, //CCW 6
};

__xdata const WORD dial_keymap[] = {
  0, //unused
  KEYBOARD_X, //clockwise
  KEYBOARD_Z, //counter-clockwise
};

//********************  INIT ***********************

void main_init() {
  REVCTL = 3;
  SETIF48MHZ();
  SETCPUFREQ(CLK_48M); //Set CPU to 48MHz

  /*
    PA0 = IR
    PA1-PA4 = Not sure, reported as flags in keys report of stock firmware (last byte), alongside FN key
    PA5 = LCD, E
    PA6 = LCD, RS
    PA7 = LCD, RW

    PB0-PB1 = Dial
    PB2-PB5 = Spring-loaded shuttle/jog wheel
    PB6-PB7 = No clue, though they are referenced in the stock firmware.

    PC0-PC7 = LCD, DATA

    PD0-PD4 = button matrix
    PD5 = recording LED
    PD6 = network LED
    PD7 = FN button LED

    PE0-PE7 = button matrix
  */
  OEA = 0xE0;
  OEB = 0x3F;
  OEC = 0xFF;
  OED = 0xFF;
  OEE = 0x3;

  //turn recording/network LEDs off (they are inverted)
  PD5 = 1;
  PD6 = 1;

  EP1OUTBUF[0] = 0;

  lcd_init();

  ir_init();
  ir_start();
  buttons_init();
}

void send_key_report ( WORD key ) {
  EP1INBUF[0] = 1; //Report ID
  EP1INBUF[1] = key;
  EP1INBC = 2;
}

WORD button_pressed = 0;
WORD last_shuttle = 0;
WORD last_dial = 0;

void main_loop() {
  WORD mode, address, command;
  BOOL toggle;

  if( !(EP1INCS & bmEPBUSY) )
  {
    WORD button = get_button ();
    if ( button != button_pressed ) {
      //buttons
      if ( button ) {
        send_key_report ( button_keymap[button] );
        button_pressed = button;
      } else {
        send_key_report ( 0 );
        button_pressed = button;
      }
    } else if ( !button_pressed && got_ir ) {
      //infrared
      if ( decode_ir ( &mode, &toggle, &address, &command ) ) {
        button = remote_key_map ( command );
        if ( button ) {
          send_key_report ( button );
          while ( EP1INCS & bmEPBUSY ) {}
          send_key_report ( 0 );
        }
      }
      ir_start ();
    } else {
      //shuttle/jog wheel (the spring-loaded knob)
      WORD shuttle = ( IOB & 0x3C ) >> 2;
      if ( shuttle != last_shuttle ) {
        WORD key = shuttle_keymap[shuttle];
        if ( key ) {
          send_key_report ( key );
          while ( EP1INCS & bmEPBUSY ) {}
          send_key_report ( 0 );
        }
        last_shuttle = shuttle;
      } else {
        //dial
        WORD dial = IOB & 3;
        if ( dial == 0 && last_dial ) {
          send_key_report ( dial_keymap[last_dial] );
          while ( EP1INCS & bmEPBUSY ) {}
          send_key_report ( 0 );
        }
        last_dial = dial;
      }
    }
  }
  if ( !(EP1OUTCS & bmEPBUSY) ) {
    //report id
    switch ( EP1OUTBUF[0] ) {
      //keyboard LEDs
      case 2:
        PD7 = 0;
        //caps lock (FN button) LED
        if ( EP1OUTBUF[1] & 2 )
          PD7 = 1;

      break;
      //LCD
      case 3:
        lcd_goto ( 0, EP1OUTBUF[1] );
        EP1OUTBUF[10] = 0;
        lcd_write_string ( &EP1OUTBUF[2] );
      break;
      //recording & network LEDs
      case 4:
        PD5 = !(EP1OUTBUF[1] & 1);
        PD6 = !(EP1OUTBUF[1] & 2);
      break;
    }
    //acknowledge, regardless...
    EP1OUTBC = 0;
  }
}

