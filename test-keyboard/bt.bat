zcc +cpm -clib=8080 testkb.c -o testkb.com --list
zcc +cpm -clib=8080 kbits.c -o kbits.com --list
zcc +cpm -clib=8080 ioread.c -o ioread.com --list
zcc +cpm -clib=8080 ..\vti.c console.c write.c -o write.com -I.. --list
zcc +cpm -clib=8080 ..\vti.c console.c interrupt.c -o interrupt.com -I.. --list



