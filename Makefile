#CC := arm-linux-gcc
#CC := arm-linux-ar

#CC := g++
#AR := ar

CC := unicore32-linux-g++
AR := unicore32-linux-ar
#AR := ar
librgb2gry: r2g.o
#	${CC} r2g.c -o rgb2grey
	${AR} -crv librgb2gry.a r2g.o
clean:
	rm *.a *.o
