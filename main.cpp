/*
The MIT License (MIT)

Copyright (c) 2018 Linker3000 (N. Kendrick - linker3000-at-gmail-dot-com).

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

Proof of concept - BBC micro:bit SPI functionality to send serial data to 
a 74HC595 shift register.

Version: 0.1 NK 04-Feb-2018

Connections:

Microbit        Function    74HC595
   P0           MOSI        Pin 14 (SER)
   P1           CE          Pin 12 (RCLK)
   P2           SER         Pin 11 (SRCLK)
   GND          ~CE         Pin 8, Pin 13 (~CE)
   VCC (3.3v)   VCC         Pin 16
   
   
Connect LEDs to 74HC595 outputs (via 470Ohm-1K resistors) to see output.
For stability, connect a 0.1uF capacitor between VCC and GND on the 74HC595
chip, as close to its body as possible.

*/

//#include "mbed.h"
#include "MicroBit.h"
#include "nkpins.h"

MicroBit uBit;

// Moving SPI pins we want to use to the large P0 etc connections. 
// miso remains on the default pin definition as we're not using it here

SPI spi(mbit_p0, mbit_p15, mbit_p2); // mosi, miso, sclk
DigitalOut cs(mbit_p1); //Chip select pin for the shift register

//DigitalOut myled(p1); 

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    //SPI setup
    // Chip must be deselected
    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8,3);
    spi.frequency(1000000);
 
    // Select the device by setting chip select low
    cs = 0;

    // Load up the shift register
    // Send 0x8f, the command to read the WHOAMI register
    while(1){
    cs = 1;
    spi.write(0x55);

    cs = 0;
    wait(0.5);
    cs = 1;
    spi.write(0xAA);
    cs = 0;
    wait(0.5);
    }
    
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

