all: carGame.elf

CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L/opt/ti/msp430_gcc/include -L../lib

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar

COMMON_OBJECTS  = lcdutils.o lcddraw.o font-5x7.o font-8x12.o buzzer.o scoreAsm.o score.o

lcdutils.o: lcdutils.c lcdutils.h
	${CC} ${CFLAGS} -c lcdutils.c

lcddraw.o: lcddraw.c lcddraw.h
	${CC} ${CFLAGS} -c lcddraw.c

font-5x7.o: font-5x7.c
	${CC} ${CFLAGS} -c font-5x7.c

font-8x12.o: font-8x12.c
	${CC} ${CFLAGS} -c font-8x12.c

buzzer.o: buzzer.c
	${CC} ${CFLAGS} -c buzzer.c

scoreAsm.o: scoreAsm.s
	${CC} ${CFLAGS} -c scoreAsm.s

score.o: score.c score.h
	${CC} ${CFLAGS} -c score.c

carGame.o: carGame.c
	${CC} ${CFLAGS} -c carGame.c

carGame.elf: ${COMMON_OBJECTS} carGame.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer

load-car: carGame.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf