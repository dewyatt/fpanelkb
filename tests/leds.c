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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hidapi.h"

void leds_write ( hid_device *handle, int recording, int network ) {
  unsigned char buffer[2];
  //byte 0 is the Report ID (4)
  buffer[0] = 0x4;
  //byte 1 is the state of the LEDs
  buffer[1] = 0;
  if ( recording )
    buffer[1] |= 1;

  if ( network )
   buffer[1] |= 2;

  if ( hid_write ( handle, buffer, 2 ) == -1 )
    fprintf ( stderr, "Warning: hid_write failed\n" );
}

bool parse_bool ( const char *s, bool *value ) {
  if ( strcmp ( s, "1" ) == 0 ) {
    *value = true;
    return true;
  } else if ( strcmp ( s, "0" ) == 0 ) {
    *value = false;
    return true;
  }
  return false;
}

int main ( int argc, char* argv[] )
{
  hid_device *handle;
  bool recording, network;

  if ( argc != 3
        || !parse_bool ( argv[1], &recording)
        || !parse_bool ( argv[2], &network ) ) {
    fprintf ( stderr, "Usage: %s <recording> <network>\n", argv[0] );
    return 1;
  }
	handle = hid_open ( 0x04b4, 0xa002, NULL );
  if ( !handle ) {
    fprintf ( stderr, "Failed to open device\n" );
    return 1;
  }
  leds_write ( handle, recording, network );
  hid_close ( handle );
  return 0;
}

