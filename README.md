# BBCSDL

For the original BBCSDL please go to https://github.com/rtrussell/BBCSDL

This fork is part of an attempt to implement BBC Basic on a Raspberry Pi Pico.
For discussion see https://www.raspberrypi.org/forums/viewtopic.php?f=144&t=316761

It includes work by:

* R. T. Russell
* Eric Olson
* Myself

Apologies to anyone else I omitted.

There are two somewhat divergent lines of development:

1.  Console Version: To be able to use the Pico as a microcontroller programmed in BBC Basic.
    User interaction (if any) would be via a console interface over USB or serial.
    Includes program and data storage either in Pico flash and/or an attached SD card.
    This is in a fairly advanced state of development.

2.  GUI Version: To be able to use the Pico as a computer programmable in BBC Basic with input
    by an attached USB keyboard and display on an attached VGA monitor. This
    development is aimed at a Pico attached to a VGA demonstration board as per
    chapter 3 of
    [Hardware design with RP2040](https://datasheets.raspberrypi.org/rp2040/hardware-design-with-rp2040.pdf)
    or the commercial version
    [Pimoroni Pico VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base).
    This is now also fairly advanced, although there are some issues to be resolved and
    more testing required.

The following limitations are noted:

1.  HIMEM-PAGE=128K with an additional 41K has reserved for CALL and
    INSTALL libraries and the CPU stack.

2.  m0FaultDispatch is linked in by default.  This license for this
    library is free for hobby and other non-commercial products.  If
    you wish to create a commercial version of the program contained
    here, please add -DFREE to the CMakeLists.txt file.

3.  All programs in tests and the root filesystem have been tested
    and appear to work.  However, any remaining bugs are more likely
    to be related to the Pico port.  As always this is open source
    with expressly no warranty provided.

4.  There is are known buffer overflows in the wrappers appearing in
    lfswrap.c which are triggered when a filename path grows to be
    greater than 256 characters.  Please don't do that.

This project includes source from various locations with difference licenses. See the
various LICENSE.txt files.

## Build Instructions - Console Version

To build this for the Pico, make sure you have the SDK installed and the
tinyusb module installed.

Ensure that the environment variable PICO_SDK_PATH points to the path where the SDK is installed.
Then type:

     $ cd console/pico
     $ make

To build for hardware other than a Pico (assuming the hardware is supported by the SDK) use:

     $ cd console/pico
     $ make BOARD=...

At this point the files bbcbasic.uf2 and filesystem.uf2 should be in the build directory.

Plug a Pico into the USB port while holding the boot button and then assuming developing
on a Raspberry Pi with Raspberry Pi OS:

     $ cp -v bbcbasic.uf2 /media/pi/RPI-RP2

Repeat the process for filesystem.uf2

If using USB, connect as

      $ picocom /dev/ttyACM0

Or if using Raspberry Pi serial port, connect as

      $ picocom -b 115200 /dev/ttyS0

to use the interpreter. If using a serial connection tap the Enter key one or two times
to initiate the connection. The onboard LED will flash while awaiting connection.

Note that you may use minicom as well, however, minicom will not display color correctly
or resize the terminal window for the different MODE settings in BBC Basic.

## Build Instructions - GUI Version

Ensure that the environment variable PICO_SDK_PATH points to the path where the SDK is installed
and PICO_EXTRAS_PATH points to where these are installed. Then type:

     $ cd bin/pico
     $ make

To build for hardware other than a Pico on a VGA Demo Board (assuming the hardware is supported by
the SDK) use:

     $ cd console/pico
     $ make BOARD=...

Plug a Pico into the USB port while holding the boot button and then assuming developing
on a Raspberry Pi with Raspberry Pi OS:

     $ cp -v bbcbasic.uf2 /media/pi/RPI-RP2

Repeat the process for filesystem.uf2

### Usage Notes

The code has been designed for use with the VGA demonstration board.
To use (once the Pico has been programmed):

* Power should be applied to the micro-USB socket on the demo board.
* The keyboard should be connected to the USB socket on the Pico itself.
A USB to micro-USB adaptor will be required. The Pico is somewhat selective
as to which keyboards work, cheap keyboards may be better.
* The SD card is used in SPI mode, and Pico GPIOs 20 & 21 are used for
serial input and output. On the Pimoroni board, cut the links between
these GPIOs and SD_DAT1 & SD_DAT2. Optionally solder a 2x3 header in place
for a serial connection. From BBC Basic printer output is sent to serial.
It is currently also used for diagnostic output.
* An SD or SDHC card may be used, It should be formatted as FAT.
* Sound output (if implemented) uses the I2S DAC on the VGA demo card.
If required an amplified speaker or amplified headphones should be connected
to the DAC socket on the VGA demo board, not to the PWM socket.

#### Video Modes

The implementation currently supports 16 video modes:

Mode | Colours |   Text  | Graphics  | Letterbox
-----|---------|---------|-----------|----------
   0 |     2   | 80 x 32 | 640 x 256 |     Y
   1 |     4   | 40 x 32 | 320 x 256 |     Y
   2 |    16   | 20 x 32 | 160 x 256 |     Y
   3 |     2   | 80 x 25 | 640 x 225 |
   4 |     2   | 40 x 32 | 320 x 256 |     Y
   5 |     4   | 20 x 32 | 160 x 256 |     Y
   6 |     2   | 40 x 25 | 320 x 225 |
   7 |     8   | 40 x 25 | Teletext  |
   8 |     2   | 80 x 30 | 640 x 480 |
   9 |     4   | 40 x 30 | 320 x 480 |
  10 |    16   | 20 x 30 | 160 x 480 |
  11 |     2   | 80 x 25 | 640 x 450 |
  12 |     2   | 40 x 30 | 320 x 480 |
  13 |     4   | 20 x 30 | 120 x 480 |
  14 |     2   | 40 x 25 | 320 x 450 |
  15 |    16   | 40 x 30 | 320 x 240 |

Modes 0-7 reproduce those from the BBC Micro. Modes 0-2, 4 & 5 only have 256 rows of pixels
which are displayed in the centre of the monitor so may appear squashed.

Modes 3 & 6 can also display graphics.

Except for Mode 7, colours 8-15 are high intensity rather than flashing.

The generally most useful modes are mode 8, which is a monochrome display with the highest resolution,
and mode 15, which is a 16 colour mode with square pixels. The default mode on startup is mode 8.

#### Screen Refresh

By default the commands "*refresh [off|on]" do nothing. However one of two different implementations
may be enabled.

"*refresh buffer" enables double buffering of the display. In this mode turning refresh off hides any
screen changes until a "*refresh" or "*refresh on" command. For low resolution modes 4-7 or 12-14
both buffers fit within the statically allocated video storage. However for the higher resolution modes
a second buffer is allocated above HIMEM and above any INSTALLed libraries. It will probably be
necessary to lower HIMEM to make sufficient room for this second buffer.

"*refresh queue" enables an alternate mode in which, while refreshing is turned off, all VDU
requests are stored in a queue and then processed as quickly as possible when refreshing is enabled.
The VDU queue is stored above HIMEM and any INSTALLed libraries. However the size of the queue is
typically smaller than a second frame buffer. The size of the queue may be specified by a decimal
number following "queue".

"*refresh none" disables "refresh [off|on]".

#### User defined characters

Command **VDU 23,...** may be used to define user character shapes. The following points should be
noted:

* Character codes are divided into blocks of 32 characters.
* The first user defined character in a block allocates memory for all the characters in the block.
* The first character block overwrites szCmdLine, which has minimal utility for the Pico.
* PAGE has to be raised (by 256 bytes per block) if more than block of user defined characters is required.

#### Thumb Assembler

There is a built-in assembler for the ARM v6M instruction set as supported by the Pico.
By default the assembler uses "Unified" syntax. However including the following pseudo-op

   syntax d

enables the following extensions to the allowed syntax:

* If an opcode takes three arguments and the first and second are the same, then the first may be omitted.
* If the 's' suffix (indicating affects status flags) is omitted from the opcode, but the parameters are
  only valid for a status affecting instruction, then that is assumed.

The extensions may be disabled again by specifying:

    syntax u

### Missing features & Qwerks

The Pico implementation is missing features compared to the BBCSDL implementation
on full operating systems. The limitations include:

* Memory is limited. Complex expressions or deeply nested routines may result in the CPU stack
reaching HIMEM or any INSTALLed libraries or refresh buffers. Usually this will result in either
"Expression evaluation too deep!" or "Recursion too deep!" error message. It may be possible to
resolve this by lowering HIMEM. It may, however, be possible for the situation to go unnoticed
in which case the Pico may crash and have to be reset.
* High resolution text (VDU 5) is implemented, but it does not scroll, instead wraps
around back to the top of the screen. New text overlays any existing rather than
replacing it.
page or scroll mode.
* PLOT modes 168-191 & 208+ are not implemented.
* Loading and saving bitmapped images is not implemented.
* There is no double-buffering of the display. Any updates are visible immediately.

### TO DO

* Sort out the memory map for this mode - DONE but needs revisiting
* Enhance text processing (viewports) - DONE
* Implement teletext mode - DONE
* Implement graphics - DONE
* Improved keyboard processing - DONE
* High resolution text - DONE?
* Implement GET(X,Y) - DONE
* Plotting: fill, circle, ellipse - DONE
* Plotting: arc - DONE
* Sound - DONE
* User defined characters - DONE
* Plotting: segment & sector - Probaby will not do, too many cases. Can sometimes be achieved with arc & fill.
* 800x600 VGA output (currently only 640x480)?
* Testing - Lots of it
* Documentation

## Custom Builds

The makefiles (above) produce two standard builds. Custom builds can be performed using CMake.

     $ mkdir build
     $ cd build
     $ cmake [options] -S ../mcu/pico -B .
     $ make

Note the stop (current folder) after the -B flag.

The following options may be specified with the cmake command:

* -DPICO_BOARD=vgaboard if using the VGA demo board, or other board as appropriate.
* -DSTDIO=... to select the user interface:
  * -DSTDIO=USB+UART for the console on both USB and UART.
  * -DSTDIO=USB for the basic console on USB.
  * -DSTDIO=UART for the basic console on UART.
  * -DSTDIO=PICO for the GUI version with input via USB keyboard and output via VGA monitor.
* -DLFS=Y to include storage on Pico flash or -DLFS=N to exclude it.
* -DFAT=Y to include storage on SD card or -DFAT=N to exclude it.
* -DSOUND=Y to include sound support or -DSOUND=N to exclude it.
* Other cmake options if required.
