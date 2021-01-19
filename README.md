# VTI Lib

C library (for **Z88DK**) for VTI graphics board for S100 computers (IMSAI, Altair, Poly88,...). Made for CP/M projects.

## VTI (Video Terminal Interface)

VTI is a S100 "graphics" board, made for "Poly88" system. In this video it's working on an IMSAI 8080.

Here is the board description: http://www.s100computers.com/Hardware%20Folder/PolyMorphic/Video%20Board/Video%20Board.htm

It uses a memory-mapped I/O so writing a single byte to memory would make a character appear on screen. It has a 64x16 chars resolution, and it has a set of "2x3" block characters, so it can reach a 128x48 "pixels" resolution.

Starting address is $F800 by default, but it can be changed using dip-switches. In the IMSAI 8080 in this video it has been changed to $E800 and has some glitches due to a broken RAM chip, which has to be replaced.

## Compiling demo

There is a **build.bat** file which does all the work with **Z88DK**, which is simply:

```$ zcc +cpm -clib=8080 demo.c vti.c -o demo.com```

## Launching demo

By default, the start address is **$F800**. To use a alternatively **$E800**, use a dummy parameter:

```$ demo -a```

This will make the demo use $E800 as start address for the graphics card.

## Runnning demo in a video

[![running-demo](https://img.youtube.com/vi/AEd7Ui9pJeU/0.jpg)](https://www.youtube.com/watch?v=AEd7Ui9pJeU)
