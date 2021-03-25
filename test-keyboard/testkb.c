#include <stdio.h>
//__sfr __at (0xe8) VTI_KEYBOARD_PORT;

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

// register a key press and counts the duration of the STROBE bit
// returns in HL the steps counted
unsigned char read_strobe_duration() {
    #asm

    ; wait until a key is pressed
loop1:
    in   a,(0xe8)
    rla
    jp   nc, loop1

    ; reset counter
    ld   hl,0

loop2:   // 0x026b, the loop is 31 T-states on the Z80 (and 8080?)
    in   a,(0xe8)
    inc  hl
    rla
    jp   c, loop2

    ret
    #endasm
}

unsigned char vti_key_ascii() {
    return read_port() & 0x7f;   // filter STROBE bit, returns the ASCII part
}

unsigned char vti_keyboard_pressed() {
    return read_port() & 0x80;   // check if STROBE bit is ON
}

unsigned char c;
unsigned char ch;
unsigned int strobe_len;

void main(void) {
    set_port(0xe8);
    printf("Test keyboard and strobe duration\r\nPress ESC to exit\r\n");

    while(1) {
        // waits for a key press and counts the STROBE duration
        // the count loop lasts 31 T-states
        unsigned int strobe_len = read_strobe_duration() * 31;
        // reads the key pressed
        char c = vti_key_ascii();
        ch = c;
        if(ch<32) ch=32;
        printf("ASCII %3d char '%c' strobe dur=%u \r\n", c, ch, strobe_len);
        if(c==27) break;
    }

    printf("done\r\n");
}
