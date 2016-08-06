PROG	= main
OBJ	= gol_com.o gol_par.o main.o
HDR	= gol.h

CC	= gcc
CFLAGS	= -W -Wall -march=native -O3
LDFLAGS	= -static

$(PROG): $(OBJ)

$(OBJ): $(HDR)

clean:
	rm -f $(PROG) $(OBJ)
