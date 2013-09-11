;;;
; The MIT License (MIT)
;
; Copyright (c) 2013 Daniel Wyatt
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
;;;

.module DEV_DSCR 

; descriptor types
; same as setupdat.h
DSCR_DEVICE_TYPE=1
DSCR_CONFIG_TYPE=2
DSCR_STRING_TYPE=3
DSCR_INTERFACE_TYPE=4
DSCR_ENDPOINT_TYPE=5
DSCR_DEVQUAL_TYPE=6

; for the repeating interfaces
DSCR_INTERFACE_LEN=9
DSCR_ENDPOINT_LEN=7

; endpoint types
ENDPOINT_TYPE_CONTROL=0
ENDPOINT_TYPE_ISO=1
ENDPOINT_TYPE_BULK=2
ENDPOINT_TYPE_INT=3

    .globl	_dev_dscr, _dev_qual_dscr, _highspd_dscr, _fullspd_dscr, _dev_strings, _dev_strings_end
    .globl _hid_dscr, _hid_report_dscr, _hid_report_dscr_end
; These need to be in code memory.  If
; they aren't you'll have to manully copy them somewhere
; in code memory otherwise SUDPTRH:L don't work right
    .area	DSCR_AREA	(CODE)

_dev_dscr:
	.db	dev_dscr_end-_dev_dscr    ; len
	.db	DSCR_DEVICE_TYPE		  ; type
	.dw	0x0002					  ; usb 2.0
	.db	0x00  					  ; class
	.db	0x00					  ; subclass
	.db	0x00					  ; protocol
	.db	64						  ; packet size (ep0)
	.dw	0xB404					  ; vendor id 
	.dw	0x02a0					  ; product id
	.dw	0x0100					  ; version id
	.db	1		                  ; manufacturure str idx				
	.db	2				          ; product str idx	
	.db	0				          ; serial str idx 
	.db	1			              ; n configurations
dev_dscr_end:

_dev_qual_dscr:
	.db	dev_qualdscr_end-_dev_qual_dscr
	.db	DSCR_DEVQUAL_TYPE
	.dw	0x0002                              ; usb 2.0
	.db	0x00
	.db	0x00
	.db	0x00
	.db	64                                  ; max packet
	.db	1									; n configs
	.db	0									; extra reserved byte
dev_qualdscr_end:

_highspd_dscr:
	.db	highspd_dscr_end-_highspd_dscr      ; dscr len											;; Descriptor length
	.db	DSCR_CONFIG_TYPE
    ; can't use .dw because byte order is different
	.db	(highspd_dscr_realend-_highspd_dscr) % 256 ; total length of config lsb
	.db	(highspd_dscr_realend-_highspd_dscr) / 256 ; total length of config msb
	.db	1								 ; n interfaces
	.db	1								 ; config number
	.db	0								 ; config string
	.db	0x80                             ; attrs = bus powered, no wakeup
	.db	0x32                             ; max power = 100ma
highspd_dscr_end:

; all the interfaces next 
; NOTE the default TRM actually has more alt interfaces
; but you can add them back in if you need them.
; here, we just use the default alt setting 1 from the trm
	.db	DSCR_INTERFACE_LEN
	.db	DSCR_INTERFACE_TYPE
	.db	0				 ; index
	.db	0				 ; alt setting idx
	.db	2				 ; n endpoints	
	.db	0x03			 ; class (HID)
	.db	0x00
	.db	0x00
	.db	0	             ; string index	

_hid_dscr:
  .db   0x09      ; length
  .db   0x21      ; type: HID
  .db   0x010,0x01      ; release: HID class rev 1.1
  .db   0x00      ; country code (none)
  .db   0x01      ; number of HID class descriptors to follow
  .db   0x22      ; report descriptor type (HID)
  .db   (_hid_report_dscr_end - _hid_report_dscr) ; length of HID descriptor
  .db   0x00
hid_dscr_end:

; endpoint 1 out
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x01				;  ep1 dir=out and address
	.db	ENDPOINT_TYPE_INT	; type
	.db	0x40				; max packet LSB
	.db	0x00				; max packet size
	.db	0x0A				; polling interval
      
; endpoint 1 in 
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x81				;  ep1 dir=in and address
	.db	ENDPOINT_TYPE_INT	; type
	.db	0x40				; max packet LSB
	.db	0x00				; max packet size
	.db	0x0A				; polling interval

highspd_dscr_realend:

.even
_fullspd_dscr:
	.db	fullspd_dscr_end-_fullspd_dscr      ; dscr len
	.db	DSCR_CONFIG_TYPE
    ; can't use .dw because byte order is different
	.db	(fullspd_dscr_realend-_fullspd_dscr) % 256 ; total length of config lsb
	.db	(fullspd_dscr_realend-_fullspd_dscr) / 256 ; total length of config msb
	.db	1								 ; n interfaces
	.db	1								 ; config number
	.db	0								 ; config string
	.db	0x80                             ; attrs = bus powered, no wakeup
	.db	0x32                             ; max power = 100ma
fullspd_dscr_end:

; all the interfaces next 
; NOTE the default TRM actually has more alt interfaces
; but you can add them back in if you need them.
; here, we just use the default alt setting 1 from the trm
	.db	DSCR_INTERFACE_LEN
	.db	DSCR_INTERFACE_TYPE
	.db	0				 ; index
	.db	0				 ; alt setting idx
	.db	2				 ; n endpoints	
	.db	0x03			 ; class (HID)
	.db	0x00
	.db	0x00
	.db	0	             ; string index	

; endpoint 1 out
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x01				;  ep1 dir=out and address
	.db	ENDPOINT_TYPE_INT	; type
	.db	0x40				; max packet LSB
	.db	0x00				; max packet size=64 bytes
	.db	0x0A				; polling interval
      
; endpoint 1 in 
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x81				;  ep1 dir=in and address
	.db	ENDPOINT_TYPE_INT	; type
	.db	0x40				; max packet LSB
	.db	0x00				; max packet size=64 bytes
	.db	0x0A				; polling interval

fullspd_dscr_realend:

_hid_report_dscr:
  .db 0x05, 0x01                    ; USAGE_PAGE (Generic Desktop)
  .db 0x09, 0x06                    ; USAGE (Keyboard)
  .db 0xa1, 0x01                    ; COLLECTION (Application)

                                    ;;;keys
  .db 0x05, 0x07                    ;   USAGE_PAGE (Keyboard)
  .db 0x85, 0x01                    ;   REPORT_ID (1)
  .db 0x75, 0x08                    ;   REPORT_SIZE (8)
  .db 0x95, 0x01                    ;   REPORT_COUNT (1)
  .db 0x15, 0x00                    ;   LOGICAL_MINIMUM (0)
  .db 0x26, 0x81, 0x00              ;   LOGICAL_MAXIMUM (129)
  .db 0x19, 0x00                    ;   USAGE_MINIMUM (Reserved (no event indicated))
  .db 0x29, 0x81                    ;   USAGE_MAXIMUM (Keyboard Volume Down)
  .db 0x81, 0x00                    ;   INPUT (Data,Ary,Abs)

                                    ;;;keyboard LEDs (we only use caps lock, for FN)
  .db 0x05, 0x08                    ;   USAGE_PAGE (LEDs)
  .db 0x15, 0x00                    ;   LOGICAL_MINIMUM (0)
  .db 0x25, 0x01                    ;   LOGICAL_MAXIMUM (1)
  .db 0x85, 0x02                    ;   REPORT_ID (2)
  .db 0x75, 0x01                    ;   REPORT_SIZE (1)
  .db 0x95, 0x03                    ;   REPORT_COUNT (3)
  .db 0x19, 0x01                    ;   USAGE_MINIMUM (Num Lock)
  .db 0x29, 0x03                    ;   USAGE_MAXIMUM (Scroll Lock)
  .db 0x91, 0x02                    ;   OUTPUT (Data,Var,Abs)
                                    ;;;padding (make led field 1 byte)
  .db 0x75, 0x05                    ;   REPORT_SIZE (1)
  .db 0x95, 0x05                    ;   REPORT_COUNT (5)
  .db 0x91, 0x03                    ;   OUTPUT (Cnst,Var,Abs)

                                    ;;;lcd (first byte = line(0/1), 8 bytes are character data)
  .db 0x06, 0x00, 0xff              ;   USAGE_PAGE (Vendor Defined Page 1)
  .db 0x09, 0x01                    ;   USAGE (Vendor Usage 1)
  .db 0x85, 0x03                    ;   REPORT_ID (3)
  .db 0x75, 0x08                    ;   REPORT_SIZE (8)
  .db 0x95, 0x09                    ;   REPORT_COUNT (9)
  .db 0x92, 0x00, 0x01              ;   OUTPUT (Data,Ary,Abs,Buf)

                                    ;;;recording & network LEDs
  .db 0x05, 0x08                    ;   USAGE_PAGE (LEDs)
  .db 0x09, 0x38                    ;   USAGE (Record)
  .db 0x09, 0x2a                    ;   USAGE (On-Line)
  .db 0x91, 0x02                    ;   OUTPUT (Data,Var,Abs)
  .db 0x85, 0x04                    ;   REPORT_ID (4)
  .db 0x95, 0x01                    ;   REPORT_COUNT (1)
  .db 0x75, 0x02                    ;   REPORT_SIZE (2)
  .db 0x91, 0x03                    ;   OUTPUT (Cnst,Var,Abs)
                                    ;;;padding (make led field 1 byte)
  .db 0x75, 0x05                    ;   REPORT_SIZE (1)
  .db 0x95, 0x06                    ;   REPORT_COUNT (6)
  .db 0x91, 0x03                    ;   OUTPUT (Cnst,Var,Abs)

  .db 0xc0                           ; END_COLLECTION
_hid_report_dscr_end:

.even
_dev_strings:
; sample string
_string0:
	.db	string0end-_string0 ; len
	.db	DSCR_STRING_TYPE
    .db 0x09, 0x04 ; 0x0409 is the language code for English.  Possible to add more codes after this. 
string0end:
; add more strings here

_string1:
  .db   string1end-_string1      ;; String descriptor length
  .db   DSCR_STRING_TYPE
  .db   'A',0
  .db   'V',0
  .db   'e',0
  .db   'r',0
  .db   'M',0
  .db   'e',0
  .db   'd',0
  .db   'i',0
  .db   'a',0
string1end:

_string2:
  .db   string2end-_string2      ;; String descriptor length
  .db   DSCR_STRING_TYPE
  .db   'S',0
  .db   'A',0
  .db   '7',0
  .db   '0',0
  .db   '0',0
  .db   '1',0
string2end:

_dev_strings_end:
    .dw 0x0000  ; in case you wanted to look at memory between _dev_strings and _dev_strings_end
