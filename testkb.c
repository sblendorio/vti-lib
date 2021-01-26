#include <stdio.h>

__sfr __at (0xe8) VTI_KEYBOARD_PORT;

unsigned char vti_key_ascii() {
    return (VTI_KEYBOARD_PORT & 0x7f);
}

unsigned char vti_keyboard_pressed() {
    return (VTI_KEYBOARD_PORT & 0x80);
}

unsigned char c;

void main(void) {
    printf("Testing keyboard... press ESC to exit\r\n");

    while(1) {

        // loop if key is not pressed
        while(!vti_keyboard_pressed());

        // key is pressed, read it
        c = vti_key_ascii();
        printf("key ASCII %d code '%c' ...", c, c);

        // loop if key is still pressed
        while(vti_keyboard_pressed());

        printf(" ok\r\n");

        if(c==27) break;
    }

    printf("done\r\n");
}


