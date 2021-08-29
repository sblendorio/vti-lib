#include <vti.h>
#include "console.h"

unsigned char vti_console_X,vti_console_Y;
unsigned char vti_console_char_under_cursor;

void vti_console_hide_cursor() {
    // restore character under cursor
    vti_rawchar_at(vti_console_X, vti_console_Y, vti_console_char_under_cursor);
}

void vti_console_show_cursor() {
    // display full block
    vti_rawchar_at(vti_console_X, vti_console_Y, 0);
}

void vti_console_advance_row() {
    vti_console_Y++;
    if(vti_console_Y==VTI_HEIGHT) {
        vti_console_Y--;
        vti_scroll_up(1);
    }
}

void vti_console_putc(unsigned char c) {
    vti_console_hide_cursor();
    if(c==13) {
        // CR
        vti_console_X=0;
        vti_console_advance_row();
    }
    else if(c==12) {
        // FF (clear screen)
        vti_console_X=0;
        vti_console_Y=0;
        vti_console_char_under_cursor = 128+32;
        vti_clear_screen();
    }
    else if(c==8) {
        // BACKSPACE, CTRL-H, cursor left
        if(vti_console_X>0) {
            vti_console_X--;
        } else {
            if(vti_console_Y>0) {
               vti_console_X = VTI_WIDTH-1;
               vti_console_Y--;
            }
        }
    }
    else if(c==14) {
        // CTRL-N, cursor right
        if(vti_console_X<VTI_WIDTH-1) {
            vti_console_X++;
        } else {
            vti_console_X = 0;
            vti_console_advance_row();
        }
    }
    else if(c==15) {
        // CTRL-O, cursor up
        if(vti_console_Y>0) vti_console_Y--;
    }
    else if(c==10) {
        // LF, CTRL-J, cursor down
        vti_console_advance_row();
    }
    else if(c==127) {
        // BACKSPACE or DEL
        // moves all characters to left
        for(int t=vti_console_X+1; t<VTI_WIDTH; t++) {
            vti_rawchar_at(t-1, vti_console_Y, vti_get_rawchar_at(t, vti_console_Y));
        }
        // fills the last character
        vti_rawchar_at(VTI_WIDTH-1, vti_console_Y, 128+32);
    }
    else {
        vti_rawchar_at(vti_console_X, vti_console_Y, c|128);
        vti_console_X++;
        if(vti_console_X==VTI_WIDTH) {
            vti_console_X=0;
            vti_console_advance_row();
        }
    }

    // save character under cursor
    vti_console_char_under_cursor = vti_get_rawchar_at(vti_console_X, vti_console_Y);

    vti_console_show_cursor();
}

void vti_console_puts(char *message) {
    while(*message) {
       vti_console_putc(*message++);
    }
}

int vti_console_flash_counter = 0;

unsigned char vti_console_inputs(unsigned char *dest) {
    // save starting point where the prompt is
    int start_x = vti_console_X;

    while(1) {
        if(vti_console_flash_counter ==    0) vti_console_show_cursor();
        if(vti_console_flash_counter ==  512) vti_console_hide_cursor();
        if(vti_console_flash_counter == 1024) vti_console_flash_counter = -1;
        vti_console_flash_counter++;

        unsigned char c = vti_get_key();

        if(c!=0) {
            if(c==13 || c==27) {
                // ENTER or ESC ends line input

                // find last non space character on the line
                vti_console_hide_cursor();
                int end_x = VTI_WIDTH-1;
                while(vti_get_rawchar_at(end_x, vti_console_Y)==128+32) end_x--;

                // copies the line onto destination buffer as C string
                for(int t=start_x; t<=end_x; t++) {
                    *dest++ = vti_get_rawchar_at(t, vti_console_Y)-128;
                }
                *dest = '\0';
            }

            vti_console_putc(c);
            vti_console_flash_counter = 0;
            vti_sleep(sleep);         // waits for strobe to go down to avoid double characters
            vti_keyboard_counter = 0;

            if(c==13 || c==27) return c;
        }
    }
}
