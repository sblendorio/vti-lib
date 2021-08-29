#include <stdio.h>
#include <stdlib.h>

#include <vti.h>

#include "console.h"

void set_interrupt_vector(unsigned int vector, void *address) {
    unsigned char *ptr = (unsigned char *) (vector * 8);
    *ptr = 0xC3;
    ptr++;
    *((unsigned int *)ptr) = address;
}

void rst0() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 0h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst8() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 8h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst10() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 10h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst18() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 18h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst20() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 20h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst28() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 28h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst30() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 30h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void rst38() {
    #asm
    push af
    push bc
    push de
    push hl
    #endasm

    vti_console_puts("RST 38h triggered\r");

    #asm
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret
    #endasm
}

void main(int argc, char *argv[]) {
    unsigned int base_address;

    // use a numeric argument to set the video memory start address
    if (argc == 2) {
        base_address = (unsigned int) atoi(argv[1]);
        if(base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
    }
    else {
        return;
    }

    vti_console_putc(12); // clear screen

    vti_console_puts("Questo programma imposta tutti e 8\r");
    vti_console_puts("i vettori di interrupt dell'8080\r");
    vti_console_puts("in modo da vedere quando scattano\r\r");

    set_interrupt_vector(0, rst0);
    set_interrupt_vector(1, rst8);
    set_interrupt_vector(2, rst10);
    set_interrupt_vector(3, rst18);
    set_interrupt_vector(4, rst20);
    set_interrupt_vector(5, rst28);
    set_interrupt_vector(6, rst30);
    set_interrupt_vector(7, rst38);

    vti_console_puts("WAITING for interrupt...");

    #asm
    db $ed, $46
    #endasm

    while(1);
}

