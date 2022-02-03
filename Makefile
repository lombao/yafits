CC=gcc

all:	fits

fits:  main.o fitsfile.o fitsheader.o fitsdata.o topng.o fitsimage.o
		$(CC) -o $@ -Wall -O3 -lpng $^

fitsimage.o: fitsimage.c fitsimage.h fits.h
		$(CC) -c $^ -O3 -Wall
		
fitsfile.o: fitsfile.c fitsfile.h fits.h
		$(CC) -c $^ -O3 -Wall

fitsheader.o: fitsheader.c fitsheader.h fits.h
		$(CC) -c $^ -O3 -Wall
		
fitsdata.o: fitsdata.c fitsdata.h fits.h 
		$(CC) -c $^ -O3 -Wall

topng.o: topng.c topng.h fits.h
		$(CC) -c $^ -O3 -Wall -lpng

main.o: main.c fits.h
		$(CC) -c $^ -Wall 


clean:
	rm -f *.o

