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

unsigned char bits[9],pbits[9] = {'-','-','-','-','-','-','-','-', 0};
unsigned char c,last_read;
unsigned char ch;

void main(void) {
    set_port(0xe8);
    printf("Keyboard bit dump... press ESC to exit\r\n");

    bits[8] = 0;

    last_read = 69;

    while(1) {
        while(1) {
            c = read_port();
            if(c!=last_read) break;
        }
        last_read = c;

        bits[0] = c & (1<<7) ? '1' : '.';
        bits[1] = c & (1<<6) ? '1' : '.';
        bits[2] = c & (1<<5) ? '1' : '.';
        bits[3] = c & (1<<4) ? '1' : '.';
        bits[4] = c & (1<<3) ? '1' : '.';
        bits[5] = c & (1<<2) ? '1' : '.';
        bits[6] = c & (1<<1) ? '1' : '.';
        bits[7] = c & (1<<0) ? '1' : '.';

        if(bits[0] == '1' && pbits[0] == '-') pbits[0] = 'X' ;
        if(bits[1] == '1' && pbits[1] == '-') pbits[1] = 'X' ;
        if(bits[2] == '1' && pbits[2] == '-') pbits[2] = 'X' ;
        if(bits[3] == '1' && pbits[3] == '-') pbits[3] = 'X' ;
        if(bits[4] == '1' && pbits[4] == '-') pbits[4] = 'X' ;
        if(bits[5] == '1' && pbits[5] == '-') pbits[5] = 'X' ;
        if(bits[6] == '1' && pbits[6] == '-') pbits[6] = 'X' ;
        if(bits[7] == '1' && pbits[7] == '-') pbits[7] = 'X' ;

        ch = c & 0x7f;
        if(ch<32) ch=32;

        printf("%s %s raw=%3d asc=%3d ch='%c'",pbits,bits,c,c & 0x7f,ch);
        if(c&0x80) printf(" STROBE\r\n");
        else printf("\r\n");
        if(c==27 || c==(27|0x80)) break;
    }

    printf("done\r\n");
}
