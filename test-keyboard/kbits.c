#include <stdio.h>

void set_port(int port) __z88dk_fastcall {
    #asm
    ld a,l
    ld (portsmc+1),a
    #endasm
}

unsigned char read_port() __z88dk_fastcall {
    #asm
portsmc:
    in a,(0xf8)
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

unsigned char bits[9];
unsigned char c;
unsigned char ch;

void main(void) {
    set_port(0xe8);
    printf("Keyboard bit dump... press ESC to exit\r\n");

    bits[8] = 0;

    while(1) {
        c = read_port();
        bits[0] = c & (1<<7) ? '1' : '.';
        bits[1] = c & (1<<6) ? '1' : '.';
        bits[2] = c & (1<<5) ? '1' : '.';
        bits[3] = c & (1<<4) ? '1' : '.';
        bits[4] = c & (1<<3) ? '1' : '.';
        bits[5] = c & (1<<2) ? '1' : '.';
        bits[6] = c & (1<<1) ? '1' : '.';
        bits[7] = c & (1<<0) ? '1' : '.';

        ch = c & 0x7f;
        if(ch<32) ch=32;

        printf("%s  raw=%d  ascii=%d char=%c",bits,c,c & 0x7f,ch);
        if(c&0x80) printf("  STROBE\r\n");
        else printf("\r\n");
        if(c==27 || c==(27|0x80)) break;
    }

    printf("done\r\n");
}
