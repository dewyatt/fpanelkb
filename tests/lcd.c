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

#include "hidapi.h"

void lcd_write ( hid_device *handle, int line, const char *s ) {
  char buffer[10];
  int length = strlen ( s );
  int i;
  //byte 0 is the Report ID (3)
  buffer[0] = 0x3;
  //byte 1 is the LCD line (0 or 1)
  buffer[1] = line;

  //copy the string to &buffer[2]
  strncpy ( buffer + 2, s, 8 );

  //fill out the rest of the string with spaces if necessary
  for ( i = length; i < 8; i++ )
    buffer[i + 2] = ' ';

  //send it off
  if ( hid_write ( handle, (unsigned char *)buffer, 10 ) == -1 )
    fprintf ( stderr, "Warning: hid_write failed\n" );
}

int main ( int argc, char* argv[] )
{
  hid_device *handle;

  if ( argc != 3 ) {
    fprintf ( stderr, "Usage: %s <lcd_line_1> <lcd_line_2>\n", argv[0] );
    return 1;
  }
	handle = hid_open ( 0x04b4, 0xa002, NULL );
  if ( !handle ) {
    fprintf ( stderr, "hid_open failed\n" );
    return 1;
  }
  lcd_write ( handle, 0, argv[1] );
  lcd_write ( handle, 1, argv[2] );
  hid_close ( handle );
  return 0;
}

