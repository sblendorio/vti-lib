#include <stdio.h>
//__sfr __at (0xe8) VTI_KEYBOARD_PORT;

unsigned char c;

void set_port(int port) __z88dk_fastcall {
    #asm
    ld a,l
    ld (portsmc+1),a
    #endasm
}

unsigned char read_port() __z88dk_fastcall {
    #asm
portsmc:
    in a,(0x00)
    ld l,a
    ld h,0
    #endasm
}

unsigned char vti_key_ascii() {
    return read_port() & 0x7f;
}

unsigned char vti_keyboard_pressed() {
    return read_port() & 0x80;
}

void main(void) {
    set_port(0xe8);
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
