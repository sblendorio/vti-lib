#include <vti.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned int word;

byte X,Y;
byte c;

int flash_counter;

void hide_cursor() {
    vti_rawchar_at(X,Y,128+32);
}

void show_cursor() {
    vti_rawchar_at(X,Y,0);
}

void advance_Y() {
    Y++;
    if(Y==VTI_HEIGHT) {
        Y--;
        vti_scroll_up(1);
    }
}

void vti_putc(byte c) {
    hide_cursor();
    if(c==13) {
        // CR
        X=0;
        advance_Y();
    }
    else if(c==8||c==127) {
        // BACKSPACE or DEL
        if(X>0) X--;
    }
    else {
        vti_rawchar_at(X,Y,c|128);
        X++;
        if(X==VTI_WIDTH) {
            X=0;
            advance_Y();
        }
    }
    show_cursor();
}

void put_string(char *message) {
    while(*message) {
       vti_putc(*message++);
       for(int t=0;t<600;t++) {
           t=t;
       }
    }
}

void main(int argc, char *argv[]) {
    unsigned int base_address;

    // use a numeric argument to set the video memory start address
    if (argc >= 2) {
        base_address = (unsigned int) atoi(argv[1]);
        if(base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
    }

    vti_clear_screen();

    X = 0;
    Y = 0;

    put_string("GREETINGS, PROFESSOR FALKEN\r\r");
    put_string("A STRANGE GAME.\rTHE ONLY WINNING MOVE IS\rNOT TO PLAY.\r\r");
    put_string("HOW ABOUT A NICE GAME OF CHESS?\r\r>");

    while(1) {
        while(!vti_keypressed()) {
            if(flash_counter ==    0) show_cursor();
            if(flash_counter ==  512) hide_cursor();
            if(flash_counter == 1024) flash_counter = -1;
            flash_counter++;
        }

        c = vti_key_ascii();
        while(vti_keypressed());

        if(c==27) break;
        vti_putc(c);
        flash_counter = 0;
    }

    printf("done\r\n");
}
