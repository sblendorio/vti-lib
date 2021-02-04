zcc +cpm -clib=8080 demos/demo.c vti.c -o demos/demo.com -O -SO -pragma-output:noprotectmsdos
zcc +cpm --math-dai32_8080 -clib=8080 demos/bezier.c vti.c -o demos/bezier.com -O -SO -pragma-output:noprotectmsdos
zcc +cpm -clib=8080 demos/pong.c -DVTI_KEYBOARD=1 vti.c -o demos/pong.com -O -SO -pragma-output:noprotectmsdos
zcc +cpm -clib=8080 demos/pong.c -DNOBARS=1 vti.c -o demos/ball.com -O -SO -pragma-output:noprotectmsdos
zcc +cpm -clib=8080 demos/lenna.c vti.c -o demos/lenna.com -O -SO -pragma-output:noprotectmsdos
zcc +cpm -clib=8080 demos/fonts.c vti.c -o demos/fonts.com -O -SO -pragma-output:noprotectmsdos

