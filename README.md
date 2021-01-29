# VTI Lib

C library (for **Z88DK**) for VTI graphics board for S100 computers (IMSAI, Altair, Poly88,...). Made for CP/M projects.

## VTI (Video Terminal Interface)

VTI is a S100 "graphics" board, made for "Poly88" system, but it works also with other S100 machines like Altair 8800, IMSAI 8080, ...

Here is the board description: http://www.s100computers.com/Hardware%20Folder/PolyMorphic/Video%20Board/Video%20Board.htm

It uses a memory-mapped I/O so writing a single byte to memory would make a character appear on screen. It has a 64x16 chars resolution, and it has a set of "2x3" block characters, so it can reach a 128x48 "pixels" resolution.

Starting address is $F800 by default, but it can be changed using dip-switches. In the IMSAI 8080 in this video it has been changed to $E800 and has some glitches due to a broken RAM chip, which has to be replaced.

## Sample code

Here is sort of "hello world":

    #include "vti.h"
    
    void main(void) {
      vti_set_start(0xf800); // optional since 0xf800 is the default
      vti_clear_screen();
      
      vti_line(1, 0, 0, 120, 45);   // segment with coords (0,0)-(120,45). "1" means "draw"
      
      vti_boxfill(1, 2, 3, 25, 25); // box filled with edges (2,3) and (25, 25). "1" means "draw"
      vti_boxfill(0, 3, 4, 25, 25); // as above, but "0" means "erase"
      
      vti_boxfill(2, 1, 1, 10, 10); // box filled with edges (1,1)-(10,10). "2" means "XOR" for each pixel
    }

## Function **vti_plot** and "mode" parameter

Main function of the library is `vti_plot` which has the following prototype:

    void vti_plot(char mode, int x, int y);

...where **x** and **y** are cartesian coordinates with (0,0) as top-left origin and max values equal to (127,47).
The parameter **mode** represents the way the dot is drawn, and can have the following values:

* **0 = ERASE**. It erases the dot at specified coordinates, making it *black*.
* **1 = DRAW**. It draws the dot, making it *white*.
* **2 = XOR**. It draws the dot if it's currently off (=black), and erases it if it's currently on (=white)

## List of library's functions

* vti_set_start(*\<start address\>*)
* vti_print_at(*\<x\>*, *\<y\>*, *"string"*);
* vti_clear_screen()
* vti_rawchar_at(*\<x\>*, *\<y\>*, *\<char\>*)
* vti_plot(*\<mode\>*, *\<x\>*, *\<y\>*)
* vti_line(*\<mode\>*, *\<x0\>*, *\<y0\>*, *\<x1\>*, *\<y1\>*)
* vti_ellipse_rect(*\<mode\>*, *\<x0\>*, *\<y0\>*, *\<x1\>*, *\<y1\>*)
* vti_box(*\<mode\>*, *\<x0\>*, *\<y0\>*, *\<x1\>*, *\<y1\>*)
* vti_boxfill(*\<mode\>*, *\<x0\>*, *\<y0\>*, *\<x1\>*, *\<y1\>*)
* vti_scroll_up(*\<number-of-rows\>*)
* vti_scroll_down(*\<number-of-rows\>*)
* vti_put_shape(*\<x\>*, *\<y\>*, char\*, *\<width\>*, *\<height\>*)
* *\<pixel\>* = vti_read_pixel(*\<x\>*, *\<y\>*)
* *\<ch\>* = vti_read_char(*\<x\>*, *\<y\>*)

## Compiling demos

There is a **build.bat** file which does all the work with **Z88DK**, this is a sample line:

```$ zcc +cpm -clib=8080 demo.c vti.c -o demo.com```

## Launching demos

By default, the start address is **$F800**. To use another address, pass it as argument in decimal notation:

```$ demo 59392```

This will make the demo use $E800 (59392) as start address for the graphics card.

There are the following demos (launch each of them eventually with the start address as parameter):

* **demo.com**: a bunch of sample of most function available in the library
* **ball.com**: a *bouncing ball* across the screen
* **pong.com**: the famous game, using a keyboard connected to the VTI's special slot
* **bezier.com**: drawing of the G-clef, using bezier curves

## Running demo in a video

[![running-demo](https://img.youtube.com/vi/v8e73I5sO7A/0.jpg)](https://www.youtube.com/watch?v=v8e73I5sO7A)

## Credits
Thanks to:
- [**Alois Zingl**](http://members.chello.at/~easyfilter/bresenham.html) for implementation of Bresenham's Algorithms (lines and curves drawing)
- [**Andrea Matteucci**](https://www.facebook.com/andrewmattew81/) for hardware providing and fixing
