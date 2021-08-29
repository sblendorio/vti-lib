#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <vti.h>

#include "console.h"

typedef unsigned char byte;
typedef unsigned int word;

int flash_counter;

void put_string_delay(char *message) {
    while(*message) {
       vti_console_putc(*message++);
       for(int t=0;t<300;t++) {
           t=t;
       }
    }
}

word steps;
word sleep;

unsigned char line[VTI_WIDTH+1];

void main(int argc, char *argv[]) {
    unsigned int base_address;

    // use a numeric argument to set the video memory start address
    if (argc == 4) {
        base_address = (unsigned int) atoi(argv[1]);
        if(base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
        steps = atoi(argv[2]);
        sleep = atoi(argv[3]);
    }
    else {
        //printf("Usage: write <videoram>\r\n");
        //printf("       videoram: address of VTI VRAM (59392, 49152, ...)\r\n");
        printf("Usage: write <videoram> <steps> <sleep>\r\n");
        printf("       videoram: address of VTI VRAM (59392, 49152, ...)\r\n");
        printf("       steps: read keyboard every n steps\r\n");
        printf("       sleep: time to wait after they key has registered\r\n\r\n");
        printf("Example: write 59392 20 400\r\n");
        return;
    }

    vti_console_putc(12); // clear screen

    vti_console_puts("GREETINGS, PROFESSOR FALKEN\r\r");
    vti_console_puts("A STRANGE GAME.\rTHE ONLY WINNING MOVE IS\rNOT TO PLAY.\r\r");
    vti_console_puts("HOW ABOUT A NICE GAME OF CHESS?\r\r>");

    vti_keyboard_counter_max = steps;

    while(1) {

        unsigned char c = vti_console_inputs(line);

        if(c==27) break;
        if(c==13) {
            if(strlen(line)!=0) {
                vti_console_puts("Why did you say: '");
                vti_console_puts(line);
                vti_console_puts("' ? \r");
            }
            vti_console_puts("> ");
        }
    }

    printf("done\r\n");
}

