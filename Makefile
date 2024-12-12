all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd wakedemo; make)
#	(cd circledemo; make)

doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
#	(cd circledemo; make clean)
	(cd wakedemo; make clean)
	rm -rf lib h
	rm -rf doxygen_docs/*

# carGame.elf: ${COMMON_OBJECTS} carGame.o
# 	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer

# load-car: carGame.elf
# 	msp430loader.sh carGame.elf