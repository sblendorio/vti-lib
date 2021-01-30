zcc +cpm -clib=8080 demo.c vti.c -o demo.com -O -SO
zcc +cpm --math-dai32_8080 -clib=8080 bezier.c vti.c -o bezier.com -O -SO
zcc +cpm -clib=8080 pong.c -DVTI_KEYBOARD=1 vti.c -o pong.com -O -SO
zcc +cpm -clib=8080 pong.c -DNOBARS=1 vti.c -o ball.com -O -SO
zcc +cpm -clib=8080 lenna.c -DVTI_KEYBOARD=1 vti.c -o lenna.com -O -SO

